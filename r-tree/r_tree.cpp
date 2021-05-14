#include "r_tree.h"
#include <vector>
Node* rTree::chooseSubtree(Place new_place) {
    return chooseSubtree(root, new_place);
}

Node* rTree::chooseSubtree(Node *start, Place new_place) {
    Node* chosen = nullptr;
    if (start->isLeaf()){
        return start;
    } else {
        int index;
        Point point (new_place.longitude,new_place.latitude);
        double minimal_mbr_increasing_area = DBL_MAX;
        for (int i = 0; i < start->nodes.size(); ++i) {
            if (start->nodes[i]->MBR.HowMuchIncreasesTheArea(point) < minimal_mbr_increasing_area){
                minimal_mbr_increasing_area = start->nodes[i]->MBR.HowMuchIncreasesTheArea(point);
            }
        }
        double minimal_area = DBL_MAX;
        for (int i = 0; i < start->nodes.size(); ++i) {
            if (start->nodes[i]->MBR.HowMuchIncreasesTheArea(point) == minimal_mbr_increasing_area) {
                if (start->nodes[i]->MBR.area() < minimal_area) {
                    minimal_area = start->nodes[i]->MBR.area();
                    index = i;
                }
            }
        }
        chosen = start->nodes[index];
    }
    
    return chooseSubtree(chosen, new_place);
}

//Если узел переполнен при вставке точки
void rTree::splitLeafNode(Node *curNode, Place *curPlace){

    Node *nodeParent;
    //В случае если является корнем дерева
    if(curNode == root){
        nodeParent = new Node;
        curNode->parentNode = nodeParent;
        nodeParent->nodes.push_back(curNode);
        root = nodeParent;
        root->parentNode = nullptr;
    }
        //В случае если он им не является
    else{
        nodeParent = curNode->parentNode;
    }

    Place **newPlaces = new Place*[maxCount + 1];
    for (int i = 0; i < maxCount; ++i){
        newPlaces[i] = curNode->objects[i];
    }
    newPlaces[maxCount] = curPlace;

    //Выбор оси разделения
    bool axisIsX = !splitLeafAxis(curNode, curPlace);

    Node* firstNode;
    Node* secondNode;
    Node* minimalFirstNode = new Node;
    Node* minimalSecondNode = new Node;

    double minimalOverlap = DBL_MAX;
    double minimalArea = DBL_MAX;
    double curOverlap;
    double curArea;

    if(axisIsX){
        qsort(newPlaces, maxCount + 1, sizeof(double), longAxisSort);
    }
    else{
        qsort(newPlaces, maxCount + 1, sizeof(double), latAxisSort);
    }

    //Пофиксить вероятный ужас с индексами
    //и ноды бы заново создавать. не будет работать так
    for (int j = minCount - 1; j <= maxCount - minCount; ++j) {

        firstNode = new Node;
        secondNode = new Node;

        int curPlace = 0;
        for (curPlace; curPlace <= j; curPlace++) {
            firstNode->objects.push_back(newPlaces[curPlace]);
        }
        for (curPlace; curPlace <= maxCount; curPlace++) {
            secondNode->objects.push_back(newPlaces[curPlace]);
        }

        firstNode->updateMBR();
        secondNode->updateMBR();

        curOverlap = firstNode->MBR.overlap(&secondNode->MBR);

        if(curOverlap < minimalOverlap){
            minimalFirstNode = firstNode;
            minimalSecondNode = secondNode;
            continue;
        }
        else{
            if(curOverlap == minimalOverlap){
                curArea = firstNode->MBR.area() + secondNode->MBR.area() - curOverlap;
                if(curArea < minimalArea){
                    *minimalFirstNode = *firstNode;
                    *minimalSecondNode = *secondNode;
                    minimalArea = curArea;
                }
            }
        }
        delete firstNode;
        delete secondNode;
    }
    //Бал сатаны
    std::cout << minimalFirstNode->objects.size() << std::endl;
    minimalFirstNode->nodeObjOutput();
    minimalFirstNode->MBRoutput();
    std::cout << minimalSecondNode->objects.size() << std::endl;
    minimalSecondNode->nodeObjOutput();
    minimalSecondNode->MBRoutput();
    minimalFirstNode->parentNode = curNode->parentNode;
    delete curNode;
    curNode = minimalFirstNode;
    curNode->MBRoutput();
    minimalSecondNode->parentNode = curNode->parentNode;

    if(curNode->parentNode->nodes.size() < maxCount){
        curNode->parentNode->nodes.push_back(minimalSecondNode);
        while(nullptr != nodeParent){
            std::cout << "UPDATED" << std::endl;
            nodeParent->updateMBR();
            nodeParent = nodeParent->parentNode;
        }
    }
    else{
        splitNotLeafNode(curNode->parentNode, minimalSecondNode);
    }
}

void rTree::splitNotLeafNode(Node *curNode, Node *insertedNode){

    Node *nodeParent;
    //В случае если является корнем дерева
    if(curNode == root){
        nodeParent = new Node;
        curNode->parentNode = nodeParent;
        nodeParent->nodes.push_back(curNode);
        root = nodeParent;
        root->parentNode = nullptr;
    }
    //В случае если он им не является
    else{
        nodeParent = curNode->parentNode;
    }

    //Копирование узлов
    Node **newNodes = new Node*[maxCount + 1];
    for (int i = 0; i < maxCount; ++i){
        newNodes[i] = curNode->nodes[i];
    }
    newNodes[maxCount] = insertedNode;

    bool axisIsX = !splitNotLeafAxis(curNode, insertedNode);

    Node* firstNode;
    Node* secondNode;
    Node* minimalFirstNode = new Node;
    Node* minimalSecondNode = new Node;

    double minimalOverlap = DBL_MAX;
    double minimalArea = DBL_MAX;
    double curOverlap;
    double curArea;

    for (int i = 0; i < 1; ++i) {
        sortForNotLeaf(newNodes, axisIsX, i);
        for (int j = minCount - 1; j < maxCount - minCount; ++j) {
            firstNode = new Node;
            secondNode = new Node;

            int curNode;
            for (curNode = 0; curNode <= j ; curNode++) {
                firstNode->nodes.push_back(newNodes[curNode]);
            }

            for (curNode = j;  curNode < maxCount - 1; curNode++) {
                secondNode->nodes.push_back(newNodes[curNode]);
            }

            firstNode->updateMBR();
            secondNode->updateMBR();

            curOverlap = firstNode->MBR.overlap(&secondNode->MBR);

            if(curOverlap < minimalOverlap){
                *(minimalFirstNode) = *(firstNode);
                *(minimalSecondNode) = *(secondNode);
                minimalOverlap = curOverlap;
            }
            else{
                if (curOverlap == minimalOverlap){
                    curArea = firstNode->MBR.area() + secondNode->MBR.area() - curOverlap;
                    if(curArea < minimalArea){
                        *(minimalFirstNode) = *(firstNode);
                        *(minimalSecondNode) = *(secondNode);
                        minimalArea = curArea;
                    }
                }
            }
            delete firstNode;
            delete secondNode;
        }
    }
    //Бал сатаны 2.0
    minimalFirstNode->parentNode = curNode->parentNode;
    delete curNode;
    curNode = minimalFirstNode;
    minimalSecondNode->parentNode = curNode->parentNode;

    if(curNode->parentNode->nodes.size() < maxCount){
        curNode->parentNode->nodes.push_back(minimalSecondNode);
        while(nullptr != nodeParent){
            std::cout << "UPDATED" << std::endl;
            nodeParent->updateMBR();
            nodeParent = nodeParent->parentNode;
        }
    }
    else{
        splitNotLeafNode(curNode->parentNode, minimalSecondNode);
    }
}

bool rTree::splitLeafAxis(Node *curNode, Place *curPlace){
    Place **newPlaces = new Place*[maxCount + 1];
    for (int i = 0; i < maxCount; ++i){
        newPlaces[i] = curNode->objects[i];
    }
    newPlaces[maxCount] = curPlace;

    Node firstNode;
    Node secondNode;

    double minimalPerimeter = DBL_MAX;

    //0 - ось X, 1 - ось Y
    for (int i = 0; i < 2; ++i){
        double curPerimeter = 0;

        //Сортировка массива по оси
        if(i){
            qsort(newPlaces, maxCount + 1, sizeof(Place*), latAxisSort);
        }
        else{
            qsort(newPlaces, maxCount + 1, sizeof(Place*), longAxisSort);
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

bool rTree::splitNotLeafAxis(Node *curNode, Node *insertedNode){
    Node **newNodes = new Node*[maxCount + 1];
    for (int i = 0; i < maxCount; ++i){
        newNodes[i] = curNode->nodes[i];
    }
    newNodes[maxCount] = insertedNode;

    double minimalPerimeter = DBL_MAX;

    Node* firstNode;
    Node* secondNode;
    bool chosenAxis;
    
    //X = 0, Y = 1
    for (int axis = 0; axis < 1; ++axis) {
        double curPerimeter = 0;
        for (int i = 0; i < 1; ++i) {
            firstNode = new Node;
            secondNode = new Node;

            sortForNotLeaf(newNodes, axis, i);

            //Поиск минимального периметра
            for (int j = 1; j < maxCount - minCount * 2 + 2; ++j) {
                int curNode = 0;
                for(curNode; curNode < minCount - 1 + j; curNode++){
                    firstNode->nodes.push_back(newNodes[curNode]);
                }
                for(curNode; curNode <= maxCount; curNode++){
                    secondNode->nodes.push_back(newNodes[curNode]);
                }

                firstNode->updateMBR();
                secondNode->updateMBR();

                curPerimeter += firstNode->MBR.perimeter() + secondNode->MBR.perimeter();
            }
            if(curPerimeter <= minimalPerimeter){
                minimalPerimeter = curPerimeter;
                chosenAxis = axis;
            }
            delete firstNode;
            delete secondNode;
        }
    }
    return chosenAxis;
}


void rTree::sortForNotLeaf(Node **nodesArray, int axis, int bound){
    if(bound == 0){
        if(axis == 0){
            qsort(nodesArray, maxCount + 1, sizeof(Node*), longLeftAxisSort);
        }
        else{
            qsort(nodesArray, maxCount + 1, sizeof(Node*), latLeftAxisSort);
        }
    }
    else{
        if(axis == 0){
            qsort(nodesArray, maxCount + 1, sizeof(Node*), longRightAxisSort);
        }
        else{
            qsort(nodesArray, maxCount + 1, sizeof(Node*), latRightAxisSort);
        }
    }
}


//Сортировка для массива мест

//Сравнение по оси Y (Lat)
int rTree::latAxisSort (const void *a, const void *b){
    const Place *arg1 = *(const Place **)a;
    const Place *arg2 = *(const Place **)b;
    if(arg1->latitude == arg2->latitude){
        return 0;
    }
    else{
        if(arg1->latitude < arg2->latitude){
            return -1;
        }
        return 1;
    }
}

//Сравнение по оси X (Long)
int rTree::longAxisSort (const void *a, const void *b){
    const Place *arg1 = *(const Place **)a;
    const Place *arg2 = *(const Place **)b;
    if(arg1->longitude == arg2->longitude){
        return 0;
    }
    else{
        if(arg1->longitude < arg2->longitude){
            return -1;
        }
        return 1;
    }
}


//Вспомогательные функции для быстрой сортировки узла
int rTree::latLeftAxisSort (const void *a, const void *b){
    const Node *arg1 = *(const Node**)a;
    const Node *arg2 = *(const Node**)b;
    if(arg1->MBR.getLeft().y == arg2->MBR.getLeft().y){
        return 0;
    }
    else{
        if(arg1->MBR.getLeft().y < arg2->MBR.getLeft().y){
            return -1;
        }
        return 1;
    }
}

int rTree::latRightAxisSort (const void *a, const void *b){
    const Node *arg1 = *(const Node**)a;
    const Node *arg2 = *(const Node**)b;
    if(arg1->MBR.getRight().y == arg2->MBR.getRight().y){
        return 0;
    }
    else{
        if(arg1->MBR.getRight().y < arg2->MBR.getRight().y){
            return -1;
        }
        return 1;
    }
}

int rTree::longLeftAxisSort (const void *a, const void *b){
    const Node *arg1 = *(const Node**)a;
    const Node *arg2 = *(const Node**)b;
    if(arg1->MBR.getLeft().x == arg2->MBR.getLeft().x){
        return 0;
    }
    else{
        if(arg1->MBR.getLeft().x < arg2->MBR.getLeft().x){
            return -1;
        }
        return 1;
    }
}

int rTree::longRightAxisSort (const void *a, const void *b){
    const Node *arg1 = *(const Node**)a;
    const Node *arg2 = *(const Node**)b;
    if(arg1->MBR.getRight().x == arg2->MBR.getRight().x){
        return 0;
    }
    else{
        if(arg1->MBR.getRight().x < arg2->MBR.getRight().x){
            return -1;
        }
        return 1;
    }
}



//Вставка места в дерево
void rTree::insertPlace(Place& curPlace) {
    std:: cout << "=============================================" << std::endl;
    Node *chosenNode = chooseSubtree(root, curPlace);
    Place *ptrToPlace = &curPlace;
    std:: cout << "Root MBR: ";
    root->MBRoutput();
    std::cout << "Chosen node MBR: ";
    chosenNode->MBRoutput();
    if(curPlace.name == "182"){
        std::cout << chosenNode->nodes.size() << std::endl;
    }
    std::cout << chosenNode->parentNode << std::endl;
    std::cout << chosenNode->nodes.size() << std::endl;
    std::cout << chosenNode->objects.size() << std::endl;
    //Если узел не переполнен
    if(chosenNode->objects.size() < maxCount){
        chosenNode->objects.push_back(ptrToPlace);
        while(chosenNode != nullptr){
            chosenNode->updateMBR();
            chosenNode = chosenNode->parentNode;
        }
    }
    else{
        splitLeafNode(chosenNode, ptrToPlace);
        for (int i = 0; i < root->nodes.size(); ++i) {
            std::cout << "NORM ";
        }
        std::cout << std::endl;
    }
}

std::vector<Place*> rTree::findObjectInCircle(Point center, double radius){
    Circle searchArea(center, radius);
    std::vector<Place*> result;
    if(root->isLeaf()){
        for (auto & object : root->objects) {
            if(searchArea.isInside(Point(object->longitude, object->latitude))){
                result.push_back(object);
            }
        }
    }
    else{
        for (auto & node : root->nodes) {
            if(searchArea.doesIntersect(node->MBR)){
                findObjectsInCircle(searchArea, node, result);
            }
        }
    }
    return result;
}

void rTree::findObjectsInCircle(Circle searchArea, Node* curArea, std::vector<Place*> &result){
    if(curArea->isLeaf()){
        for (auto & object : curArea->objects) {
            if(searchArea.isInside(Point(object->longitude, object->latitude))){
                result.push_back(object);
            }
        }
    }
    else{
        for (auto & node : curArea->nodes) {
            if(searchArea.doesIntersect(node->MBR)){
                findObjectsInCircle(searchArea, node, result);
            }
        }
    }
}
