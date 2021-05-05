#include "r_tree.h"

Node* rTree::chooseSubtree(Place new_place) {
    return chooseSubtree(root, new_place);
}

Node* rTree::chooseSubtree(Node *start, Place new_place) {
    Node* chosen = nullptr;
    if (start->isLeaf()){
        return start;
    } else {
        bool thereIsNoLeaves = true;
        double minimal_overlap_area = 999999999;
        int index;
        for (int i = 0; i < start->nodes.size(); ++i) {
            if (start->nodes[i].isLeaf()){
                thereIsNoLeaves = false;
                //TODO: выбрать из листков тот, у которого наименьшим образом возрастает пересечение
                //Я не очень понял что это значит. Когда разберусь - напишу.
            }
        }

        //выбираем дочерний узел с наименьшим увеличением площади MBR
        if (thereIsNoLeaves) {
            Point point (new_place.longitude,new_place.latitude);
            double minimal_mbr_area = 999999999;
            //index определен раньше. По хорошему, надо хранить все ноды, которые достигнут минимума...
            //но мне лень. может позже вектор приделаю
            for (int i = 0; i < start->nodes.size(); ++i) {
                if (start->nodes[i].MBR.HowMuchIncreasesTheArea(point) < minimal_mbr_area){
                    minimal_mbr_area = start->nodes[i].MBR.HowMuchIncreasesTheArea(point);
                    index = i;
                }
            }
            chosen = &start->nodes[index];
        }
    }
    
    return chooseSubtree(chosen, new_place);
}