#include "Circle.h"

double distance(const Point& p1, const Point& p2){
    const double Pi = std::acos(-1);
    const int R = 6371; // радиус Земли
    double lambda1 = (p1.x)*Pi/180.0;
    double lambda2 = (p2.x)*Pi/180.0;
    double phi1 = (p1.y)*Pi/180.0;
    double phi2 = (p2.y)*Pi/180.0;

    double result = 2*R*sqrt(sin((phi2-phi1)/2)*sin((phi2-phi1)/2)+cos(phi1)*cos(phi2)
                                                                   *sin((lambda2-lambda1)/2)*sin((lambda2-lambda1)/2));
    return result;
}

bool Circle::isInside(const Point &point) {
    if (distance(center, point)<=radius) return true;
    else return false;
}

bool Circle::doesIntersect(const Rectangle &rectangle) {
    if (center.x < rectangle.getRight().x && center.x > rectangle.getLeft().x) {
        if (center.y < rectangle.getRight().y && center.y > rectangle.getLeft().y) {
            return true;
        }
        if (distance(center, Point(center.x, rectangle.getRight().y)) <= radius) {
            return true;
        }
        if (distance(center, Point(center.x, rectangle.getLeft().y)) <= radius) {
            return true;
        }
    }

    if (center.y < rectangle.getRight().y && center.y > rectangle.getLeft().y){
        if (distance(center, Point(rectangle.getRight().x, center.y)) <= radius) {
            return true;
        }
        if (distance(center, Point(rectangle.getLeft().x,center.y)) <= radius) {
            return true;
        }
    }
    if (distance(center, rectangle.getRight()) <= radius) return true;
    if (distance(center, rectangle.getLeft()) <= radius) return true;
    if (distance(center, Point(rectangle.getRight().x, rectangle.getLeft().y)) <= radius) return true;
    if (distance(center, Point(rectangle.getLeft().x, rectangle.getRight().y)) <= radius) return true;
    return false;
}


