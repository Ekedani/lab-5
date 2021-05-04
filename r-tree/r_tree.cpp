#include "r_tree.h"

template<class T>
Node<T> *rTree<T>::chooseSubtree(T new_node) {
    return chooseSubtree(root);
}

template<class T>
Node<T> *rTree<T>::chooseSubtree(Node<T> *start, T new_node) {
    if (start->nodes.size()==0){
        return start;
    } else {
        Node<T>* chosen;
        //TODO: весь алгоритм вообще-то. Я просто отвлекся на прямоугольники.
    }
}