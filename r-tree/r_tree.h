#include <vector>
#include "Circle.h"
#include <cfloat>
#include "../data/place.h"
#include "Rectangle.h"
#include <iostream>

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
        if (this->MBR.isEmpty()) {
            if (!objects.empty()) {
                MBR = Rectangle(Point(objects[0]->longitude,objects[0]->latitude),
                                Point(objects[0]->longitude,objects[0]->latitude));
            } else if (!isLeaf()){
                MBR = Rectangle(nodes[0]->MBR.getLeft(), nodes[0]->MBR.getRight());
            }
        }
        if (!objects.empty()) {
            for (int i = 0; i < objects.size(); ++i) {
                MBR.extendRectangleToPoint(Point(objects[i]->longitude, objects[i]->latitude));
            }
        }
        if (!isLeaf()){
            for (int i = 0; i < nodes.size(); ++i) {
                MBR.extendRectangleToPoint(nodes[i]->MBR.getLeft());
                MBR.extendRectangleToPoint(nodes[i]->MBR.getRight());
            }
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

    //Методы, что используются во вставке элемента
    void splitNotLeafNode(Node *curNode, Node *insertedNode);
    void splitLeafNode(Node *curNode, Place curPlace);
    static bool splitLeafAxis(Node *curNode, Place curPlace);

    //Вспомогательные методы для быстрой сортировки
    static int latAxisSort (const void *a, const void *b);
    static int longAxisSort (const void *a, const void *b);
    static int longLeftAxisSort (const void *a, const void *b);
    static int longRightAxisSort (const void *a, const void *b);
    static int latLeftAxisSort (const void *a, const void *b);
    static int latRightAxisSort (const void *a, const void *b);

    bool splitNotLeafAxis(Node *curNode, Node *insertedNode);
    void sortForNotLeaf(Node **nodesArray, int axis, int bound);

    //Метод поиска, вызвается публичным
    void findObjectsInCircle(Circle searchArea, Node* curArea, std::vector<Place*> &result);

public:

    std::vector<Place*> findObjectInCircle(Point center, double radius);
    rTree(){
        root = new Node;
        root->parentNode = nullptr;
        root->updateMBR();
    }

    void showObjects(){
        if(root->isLeaf()){
            for (int i = 0; i < root->objects.size(); ++i) {
                std::cout << root->objects[i]->name << std::endl;
            }
        }
        else{
            std::cout << "ROOT IS NOT LEAF" << std::endl;
            std::cout << "NUM OF CHILDREN: " << root->nodes.size() << std::endl;
        }
    }

    Node* chooseSubtree(Place new_place);
    Node* chooseSubtree(Node* start, Place new_place); // эта штука в приватные потом переместится, т.к её вызывает метод выше.

    void insertPlace(Place& curPlace);
    void Test();
};
