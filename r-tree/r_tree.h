#include <vector>
#include <cfloat>
#include "../data/place.h"
#include "Rectangle.h"

//Узел дерева
struct Node{
    Node *parentNode;
    Rectangle MBR;
    std::vector<Place*> objects;
    std::vector<Node*> nodes;
    bool isLeaf(){
        if (nodes.empty()) return true;
        return false;
    }
    void updateMBR(){
        if (isLeaf()) return;
        MBR = Rectangle(Point(objects[0]->longitude,objects[0]->latitude),Point(objects[0]->longitude,objects[0]->latitude));
        for (int i = 1; i < objects.size(); ++i) {
            MBR.extendRectangleToPoint(Point(objects[i]->longitude,objects[i]->latitude));
        }
    }
};

//Само дерево
class rTree {
private:
    //Корень дерева
    Node *root;

    //Минимальное и максимальное количество элементов в узле
    const static int minCount = 6;
    const static int maxCount = 16;

    //Если узел переполнен при вставке точки
    void splitLeafNode(Node *curNode, Place curPlace){

        Node *nodeParent;
        //В случае если является корнем дерева
        if(curNode == root){
           nodeParent = new Node;
           curNode->parentNode = nodeParent;
           nodeParent->nodes.push_back(curNode);
           root = nodeParent;
        }
        //В случае если он им не является
        else{
            nodeParent = curNode->parentNode;
        }

        Place **newPlaces = new Place*[maxCount + 1];
        for (int i = 0; i < maxCount; ++i){
            newPlaces[i] = curNode->objects[i];
        }
        newPlaces[maxCount] = &curPlace;

        //Выбор оси разделения
        bool axisIsX = splitLeafAxis(curNode, curPlace);

        Node firstNode;
        Node secondNode;
        Node minimalFirstNode;
        Node minimalSecondNode;

        double minimalOverlap = DBL_MAX;
        double minimalArea = DBL_MAX;
        double curOverlap;
        double curArea;

        if(axisIsX){
            qsort(newPlaces, maxCount + 1, sizeof(double), latAxisSort);
        }
        else{
            qsort(newPlaces, maxCount + 1, sizeof(double), longAxisSort);
        }

        //Пофиксить вероятный ужас с индексами
        for (int j = minCount - 1; j <= maxCount - minCount; ++j) {

            int curPlace = 0;
            for (curPlace; curPlace <= j; curPlace++) {
                firstNode.objects.push_back(newPlaces[curPlace]);
            }
            for (curPlace; curPlace < maxCount - 1; curPlace++) {
                secondNode.objects.push_back(newPlaces[curPlace + 1]);
            }

            firstNode.updateMBR();
            secondNode.updateMBR();

            //Перекрытие
            //curOverlap = firstNode.MBR.overlap(secondNode.MBR)

            if(curOverlap < minimalOverlap){
                //TODO: Конструктор копирования для Node
                minimalFirstNode = firstNode;
                minimalSecondNode = secondNode;
                minimalOverlap = curOverlap;
            }
            else{
                if(curOverlap == minimalOverlap){
                    curArea = firstNode.MBR.area() + secondNode.MBR.area();
                    if(curArea < minimalArea){
                        minimalFirstNode = firstNode;
                        minimalSecondNode = secondNode;
                        minimalArea = curArea;
                    }
                }
            }
        }
        //TODO: Перераспределение дочерних
        if(curNode->parentNode->nodes.size() < maxCount){
            //TODO: Вставка и обновление MBR родителя
        }
        else{
            //TODO: Разделение родительского узла
        }
    }

    bool splitLeafAxis(Node *curNode, Place curPlace){
        Place **newPlaces = new Place*[maxCount + 1];
        for (int i = 0; i < maxCount; ++i){
            newPlaces[i] = curNode->objects[i];
        }
        newPlaces[maxCount] = &curPlace;

        Node firstNode;
        Node secondNode;

        double minimalPerimeter = DBL_MAX;

        //0 - ось X, 1 - ось Y
        for (int i = 0; i < 2; ++i){
            double curPerimeter = 0;

            //Сортировка массива по оси
            if(i){
                qsort(newPlaces, maxCount + 1, sizeof(double), latAxisSort);
            }
            else{
                qsort(newPlaces, maxCount + 1, sizeof(double), longAxisSort);
            }

            //Есть вероятность, что тут могут быть проблемы с индексами
            for (int j = 1; j <= maxCount - minCount * 2 + 2; ++j) {

                int curPlace = 0;
                for (curPlace; curPlace < minCount - 1 + j; ++curPlace) {
                    firstNode.objects.push_back(newPlaces[curPlace]);
                }
                for (curPlace; curPlace <= maxCount; ++curPlace) {
                    secondNode.objects.push_back(newPlaces[curPlace]);
                }

                firstNode.updateMBR();
                secondNode.updateMBR();
                curPerimeter += firstNode.MBR.overallPerimeter(&secondNode.MBR);

            }
            if (curPerimeter <= minimalPerimeter) {
                minimalPerimeter = curPerimeter;
            }
            if (i) {
                if (curPerimeter <= minimalPerimeter) {
                    return 1;
                } else return 0;
            }
        }
    }

    //Сравнение по оси X (Lat)
    static int latAxisSort (const void *a, const void *b){
        const Place arg1 = *(static_cast<const Place*>(a));
        const Place arg2 = *(static_cast<const Place*>(b));
        if(arg1.latitude == arg2.latitude){
            return 0;
        }
        else{
            if(arg1.latitude < arg2.latitude){
                return -1;
            }
            return 1;
        }

    }

    //Сравнение по оси Y (Long)
    static int longAxisSort (const void *a, const void *b){
        const Place arg1 = *(static_cast<const Place*>(a));
        const Place arg2 = *(static_cast<const Place*>(b));
        if(arg1.longitude == arg2.longitude){
            return 0;
        }
        else{
            if(arg1.longitude < arg2.longitude){
                return -1;
            }
            return 1;
        }
    }

public:
    Node* chooseSubtree(Place new_place);
    Node* chooseSubtree(Node* start, Place new_place); // эта штука в приватные потом переместится, т.к её вызывает метод выше.

    void insertPlace(Place& curPlace);

    //TODO: Поиск объектов в заданом радиусе (findObjectsInArea)
};
