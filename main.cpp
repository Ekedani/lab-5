#include <iostream>
#include "data/output.h"
#include "data/place_filereader.h"
#include "r-tree/r_tree.h"

using namespace std;
int main() {
    rTree tree;
    std::string address = "E:\\my-repos\\lab-5\\input.csv";
    placeFileReader file(address);
    Place *placePtr;
    while(!file.endOfFile()){
        placePtr = new Place;
        *(placePtr) = file.readALine();
        tree.insertPlace(*placePtr);
    }
    vector<Place*> places = tree.findObjectInCircle(Point(22.40971,48.89283), 30);
    for (int i = 0; i < places.size(); ++i) {
        cout << places[i]->latitude << " " << places[i]->longitude << endl;
    }
    return 0;

}
