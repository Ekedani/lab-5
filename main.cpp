#include "r-tree/r_tree.h"
#include "string"
int main() {
    rTree test;
    Place *testPlace;
    for (int i = 0; i < 100; ++i) {
        testPlace = new Place;
        testPlace->latitude = i;
        testPlace->longitude = i;
        testPlace->name = std::to_string(i);
        test.insertPlace(*testPlace);
    }
    test.showObjects();

    return 0;

}
