#include "r_tree.h"

Node* rTree::chooseSubtree(Place new_place) {
    return chooseSubtree(root, new_place);
}

Node* rTree::chooseSubtree(Node *start, Place new_place) {
    if (start->isLeaf()){
        return start;
    } else {
        Node* chosen;
        bool thereIsNoLeaves = true;
        for (int i = 0; i < start->nodes.size(); ++i) {
            if (start->nodes[i].isLeaf()){
                thereIsNoLeaves = false;
                //TODO: выбрать из листков тот, у которого наименьшим образом возрастает пересечение
            }
        }

        if (thereIsNoLeaves) {
            //TODO: выбрать узел с наименьшим увеличением площади MBR
        }
        //TODO: весь алгоритм вообще-то. Я просто отвлекся на прямоугольники.
    }
}