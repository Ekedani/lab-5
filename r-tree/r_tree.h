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
           Node newRoot;
           nodeParent = &newRoot;
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
        newPlaces[maxCount + 1] = &curPlace;

        //Дальше будет создание узлов, выбор оси и моменты, которые у нас еще не готовы
    }

    //0 - ось X, 1 - ось Y
    bool splitLeafAxis(Node *curNode, Place curPlace){
        Place **newPlaces = new Place*[maxCount + 1];
        for (int i = 0; i < maxCount; ++i){
            newPlaces[i] = curNode->objects[i];
        }
        newPlaces[maxCount + 1] = &curPlace;

        Node firstNode;
        Node secondNode;

        double minimalPerimeter = DBL_MAX;

        //0 - ось X, 1 - ось Y
        for (int i = 0; i < 2; ++i){
            double curPerimeter = 0;
            //Сортировка по осям

            //Периметры узлов во всех возможных комбинациях

            //Выбор оси в зависимости от минимального
        }

        //Возврат выбраной оси
    }
    
public:
    //TODO: сделать выбор листка, см.тело методов ниже
    Node* chooseSubtree(Place new_place);
    Node* chooseSubtree(Node* start, Place new_place); // эта штука в приватные потом переместится, т.к её вызывает метод выше.

    //TODO: Алгоритм деления узла (splitNode)

    //TODO: Собственно сама вставка (insertObject)

    //TODO: Поиск объектов в заданом радиусе (findObjectsInArea)

    //TODO: Так же, если я не ошибаюсь, R* требует регулировки некоторых параметров при splitNode
};
