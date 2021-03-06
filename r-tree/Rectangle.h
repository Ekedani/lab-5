#pragma once

#include <cfloat>
#include <cmath>

//Будет использоваться как угол ограничивающего прямоугольника
struct Point {
    double x;
    double y;
    Point(){
        x = DBL_MAX;
        y = DBL_MAX;
    }
    Point(double vx, double vy){
        x = vx;
        y = vy;
    }
};

class Rectangle {
    Point left, right; //левый нижний и правый верхний углы
public:
    Rectangle(){}
    Rectangle(Point l, Point r){
        left = l;
        right = r;
    }


    bool isEmpty();
    bool isInside(Point point);
    double area();
    double perimeter();
    double overallPerimeter(Rectangle* rec1);
    Rectangle extendRectangleToPoint(Point point);
    double HowMuchIncreasesTheArea(Point point); //насколько вырастет площадь MBR если туда засунуть point
    double overlap(Rectangle* rec1);//площадь пересечения this и rec1. Я не знаю нужно ли это, не до конца разобрался
    //но на всякий случай пусть будет
    const Point &getLeft() const;
    const Point &getRight() const;

};