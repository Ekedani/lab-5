#include "Rectangle.h"
double distance(const Point& p1, const Point& p2);

class Circle {
    Point center;
    double radius;
public:
    Circle(){}
    Circle(const Point& center, double radius){
        Circle::center = center;
        Circle::radius = radius;
    }
    bool isInside(const Point& point);
    bool doesIntersect(const Rectangle& rectangle);
};