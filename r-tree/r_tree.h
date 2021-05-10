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

        //Дальше будет создание узлов, выбор оси и моменты, которые у нас еще не готовы
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
            if(i){
                qsort(newPlaces, maxCount + 1, sizeof(double), latAxisSort);
            }
            else{
                qsort(newPlaces, maxCount + 1, sizeof(double), longAxisSort);
            }

            //Периметры узлов во всех возможных комбинациях

            //Выбор оси в зависимости от минимального
        }

        //Возврат выбраной оси
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
    //TODO: сделать выбор листка, см.тело методов ниже
    Node* chooseSubtree(Place new_place);
    Node* chooseSubtree(Node* start, Place new_place); // эта штука в приватные потом переместится, т.к её вызывает метод выше.

    //TODO: Алгоритм деления узла (splitNode)

    void insertPlace(const Place& curPlace);

    //TODO: Поиск объектов в заданом радиусе (findObjectsInArea)

    //TODO: Так же, если я не ошибаюсь, R* требует регулировки некоторых параметров при splitNode
};
