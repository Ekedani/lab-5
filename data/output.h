#pragma once

#include "../r-tree/Circle.h"
#include "place.h"
#include "vector"
#include <iostream>

class outputWriter{
public:
    static void showSearchResults(const std::vector<Place*> &results){
        if(results.empty()){
            std::cout << "Nothing found!" << std::endl;
        }
        else{
            std::cout << "The following locations have been found: " << std::endl;
            for (int i = 0; i < results.size(); ++i) {
                std::cout << i + 1 << ". " << results[i]->name << std::endl;
            }
        }
    }
};