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
        std::vector<int> chosen_nodes;
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
void rTree::splitLeafNode(Node *curNode, Place curPlace){

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
    bool axisIsX = !splitLeafAxis(curNode, curPlace);

    Node* firstNode;
    Node* secondNode;
    Node* minimalFirstNode;
    Node* minimalSecondNode;

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
        for (curPlace; curPlace < maxCount - 1; curPlace++) {
            secondNode->objects.push_back(newPlaces[curPlace + 1]);
        }

        firstNode->updateMBR();
        secondNode->updateMBR();

        //TODO: Перекрытие
        //curOverlap = firstNode.MBR.overlap(secondNode.MBR)

        if(curOverlap < minimalOverlap){
            //TODO: Конструктор копирования для Node
            *minimalFirstNode = *firstNode;
            *minimalSecondNode = *secondNode;
            minimalOverlap = curOverlap;
        }
        else{
            if(curOverlap == minimalOverlap){
                curArea = firstNode->MBR.area() + secondNode->MBR.area();
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
    minimalFirstNode->parentNode = curNode->parentNode;
    delete curNode;
    curNode = minimalFirstNode;
    minimalSecondNode->parentNode = curNode->parentNode;

    if(curNode->parentNode->nodes.size() < maxCount){
        curNode->parentNode->nodes.push_back(minimalSecondNode);
        curNode->parentNode->updateMBR();
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

    bool axisIsX = !splitLeafAxis(curNode, insertedNode);

    Node* firstNode;
    Node* secondNode;
    Node* minimalFirstNode;
    Node* minimalSecondNode;

}

bool rTree::splitLeafAxis(Node *curNode, Place curPlace){
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
int rTree::latAxisSort (const void *a, const void *b){
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

bool rTree::splitNotLeafAxis(Node *curNode, Node *insertedNode){
    //TODO: выбор оси
}

//Сравнение по оси Y (Long)
int rTree::longAxisSort (const void *a, const void *b){
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

//Вставка места в дерево
void rTree::insertPlace(Place& curPlace) {
    Node *chosenNode = chooseSubtree(root, curPlace);
    Place *ptrToPlace = &curPlace;

    //Если узел не переполнен
    if(chosenNode->nodes.size() < maxCount){
        chosenNode->objects.push_back(ptrToPlace);
        while(chosenNode != NULL){
            chosenNode->updateMBR();
            chosenNode = chosenNode->parentNode;
        }
    }
    else{
        splitLeafNode(chosenNode, curPlace);
    }
}
