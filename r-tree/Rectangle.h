//Будет использоваться как угол ограничивающего прямоугольника
struct Point {
    double x;
    double y;
    Point(){}
    Point(int vx, int vy){
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
    bool isInside(Point point);
    double area();
    Rectangle extendRectangleToPoint(Point point);
    double HowMuchIncreasesTheArea(Point point);
    double overlap(Rectangle* rec1);
};