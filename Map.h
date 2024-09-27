//
// Created by Dugua on 9/26/2024.
//

#ifndef MAP_H
#define MAP_H
#include <cstdint>
#include <iostream>
#include <list>
#include <fstream>

class Territory
{
    std::string name;
    int16_t xCoord;
    int16_t yCoord;
    std::list<Territory*> adjacentTerritories;
};

class Continent
{
    std::string name;
    int16_t score;
    std::list<Territory*> territories;
};

class Map
{
    bool isWrapable;
    bool scrollsVertically;
    bool includeWarnings;
    std::string image;
    std::string name;
    std::string author;

    std::list<Continent*> continents;

    bool validate();
};

class MapLoader
{
    std::ifstream inputFileStream;
};

#endif //MAP_H