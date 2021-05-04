#include <algorithm>
#include "Rectangle.h"

double Rectangle::overlap(Rectangle* rec1){
    //Координаты прямоугольника в пересечении
    double left_coord = std::max(this->left.x, rec1->left.x);
    double top_coord = std::min(this->right.y, rec1->right.y);
    double right_coord = std::min(this->right.x, rec1->right.x);
    double bottom_coord = std::max(this->left.y, rec1->left.y);

    //длины сторон получившегося прямоугольника
    double width = right_coord - left_coord;
    double height = top_coord - bottom_coord;

    if (width<0 || height<0) {
        return 0;
    }
    //площадь пересечения
    return width*height;
}
