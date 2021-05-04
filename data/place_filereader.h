#include <fstream>
#include <string>
#include "place.h"
#include <vector>

//TODO: Поддержка кириллицы

class placeFileReader{
private:
    std::string path;
    std::ifstream filePointer;
public:
    //Методы закрытия и открытия лучше переместить в приватные, а вместо них сделать конструкт/деструкт
    //Вот тут кстати не уверен, что будет хорошо работать
    void openFile(std::string path){
        this->path = path;
        this->filePointer.open(path);
    }

    Place readALine(){
        //Берем строку
        std::string curLine;
        Place curPlace;
        getline(filePointer, curLine);

        //Структуры для хранения информации
        std::vector<double> coords;
        std::vector<std::string> info;

        //Чтение координат
        for(int i = 0; i < 2; i++){
            std::string doubleString;
            doubleString = curLine.substr(0, curLine.find(';'));
            curLine.erase(0, curLine.find(';') + 1);
            int comaPos = doubleString.find(',');
            doubleString[comaPos] = '.';
            coords.push_back(std::stod(doubleString));
        }

        //Чтение тегов и адресов
        for(int i = 0; i < 4; i++){
            std::string infoString;
            infoString = curLine.substr(0, curLine.find(';'));
            curLine.erase(0, curLine.find(';') + 1);
            info.push_back(infoString);
        }

        //Присваивание информации возвращаемому объекту
        curPlace.latitude = coords[0];
        curPlace.longitude = coords[1];
        curPlace.type = info[0];
        curPlace.subtype = info[1];
        curPlace.name = info[2];
        curPlace.address = info[3];

        return curPlace;
    }

    void closeFile(){
        this->filePointer.close();
        this->path = "";
    }
};
