#include <iostream>
#include "data/output.h"
#include "data/place_filereader.h"
#include "r-tree/r_tree.h"

using namespace std;
int main(int argc, char** argv) {
    if (argc != 5) {
        cout << "Inappropriate number of arguments";
        return -1;
    }
    std::string address = argv[1];
    double latitude = stod(argv[2]);
    double longitude = stod(argv[3]);
    double radius = stod(argv[4]);

    rTree tree;
    placeFileReader file(address);
    Place *placePtr;
    while(!file.endOfFile()){
        placePtr = new Place;
        *(placePtr) = file.readALine();
        tree.insertPlace(*placePtr);
    }
    vector<Place*> places = tree.findObjectInCircle(Point(longitude,latitude), radius);
    outputWriter::showSearchResults(places);
    return 0;
}
