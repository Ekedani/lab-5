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
        double minimal_mbr_increasing_area = 999999999;
        std::vector<int> chosen_nodes;
        for (int i = 0; i < start->nodes.size(); ++i) {
            if (start->nodes[i]->MBR.HowMuchIncreasesTheArea(point) < minimal_mbr_increasing_area){
                minimal_mbr_increasing_area = start->nodes[i]->MBR.HowMuchIncreasesTheArea(point);
            }
        }
        double minimal_area = 999999999;
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

void rTree::insertPlace(const Place& curPlace) {

}
