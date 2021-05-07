#include <vector>
#include "../data/place.h"
#include "Rectangle.h"

//Узел дерева
struct Node{
    Rectangle MBR;
    std::vector<Place> objects;
    std::vector<Node> nodes;
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

    //Каждый узел дерева имеет минимальное (minCount) и максимальное (maxCount) количество объектов.
    //Для корректной работы алгоритмов построения дерева нужно, что бы 2 <= minCount <= maxCount / 2

    //Минимальное и максимальное количество элементов в узле
    const static int minCount = 6;
    const static int maxCount = 16;

    //Авторы R*-дерева в своей статье утверждают,
    // что наилучшая производительность данной структуры достигается при minCount = maxCount * 40%.

public:
    //TODO: сделать выбор листка, см.тело методов ниже
    Node* chooseSubtree(Place new_place);
    Node* chooseSubtree(Node* start, Place new_place); // эта штука в приватные потом переместится, т.к её вызывает метод выше.

    //TODO: Алгоритм деления узла (splitNode)

    //TODO: Собственно сама вставка (insertObject)

    //TODO: Поиск объектов в заданом радиусе (findObjectsInArea)

    //TODO: Так же, если я не ошибаюсь, R* требует регулировки некоторых параметров при splitNode
};
