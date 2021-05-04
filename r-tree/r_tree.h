//Вероятнее всего, релизить мы будем R*, так как он более эффективный, но сложно быть не должно
//Ссылка на эту дичь https://habr.com/ru/post/224965/
#include <vector>

//Будет использоваться как угол ограничивающего прямоугольника
struct Point {
    double latitude;
    double longitude;
};

//Узел дерева
template <class T>
struct Node{
    //засунуть сюда координаты углов прямоугольника ещё
    Point left, right;
    std::vector<T> objects;
    std::vector<Node<T>> nodes;
};

//Само дерево
template <class T> // T - обьекты, которые должны храниться в прямугольничках. Например точки на карте
class rTree {
private:
    //Корень дерева
    Node<T> *root;

    //Каждый узел дерева имеет минимальное (minCount) и максимальное (maxCount) количество объектов.
    //Для корректной работы алгоритмов построения дерева нужно, что бы 2 <= minCount <= maxCount / 2

    //Минимальное и максимальное количество элементов в узле
    const static int minCount = 6;
    const static int maxCount = 16;

    //Авторы R*-дерева в своей статье утверждают,
    // что наилучшая производительность данной структуры достигается при minCount = maxCount * 40%.

public:
    //TODO: Алгоритм выбора поддерева (chooseSubtree)

    //TODO: Алгоритм деления узла (splitNode)

    //TODO: Собственно сама вставка (insertObject)

    //TODO: Поиск объектов в заданом радиусе (findObjectsInArea)

    //TODO: Так же, если я не ошибаюсь, R* требует регулировки некоторых параметров при splitNode
};
