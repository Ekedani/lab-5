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

bool Rectangle::isInside(Point point) {
    if (point.x >= this->left.x && point.x <= this->right.x) {
        if (point.y >= this->left.y && point.y <= this->right.y){
            return true;
        }
    }
    return false;
}

double Rectangle::area() {
    double width = this->right.x - this->left.x;
    double height = this->right.y - this->left.y;
    return width*height;
}

Rectangle Rectangle::extendRectangleToPoint(Point point) {
    //расширяет прямоугольник с наименьшим увеличением площади, чтобы point попала в него
    Point new_left(std::min(left.x, point.x),std::min(left.y, point.y));
    Point new_right(std::max(right.x, point.x),std::max(right.y, point.y));
    Rectangle new_rectangle(new_left, new_right);
    return new_rectangle;
}

double Rectangle::HowMuchIncreasesTheArea(Point point) {
    double difference = extendRectangleToPoint(point).area() - this->area();
    return difference;
}

double Rectangle::perimeter() {
    double width = this->right.x - this->left.x;
    double height = this->right.y - this->left.y;
    return 2*(width+height);
}
