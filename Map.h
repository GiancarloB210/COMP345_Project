//
// Created by Dugua on 9/26/2024.
//

#ifndef MAP_H
#define MAP_H
#include <cstdint>
#include <iostream>
#include <list>

class Continent;

class Map
{
    bool isWrapable;
    bool scrollsVertically;
    bool includeWarnings;

    std::list<Continent*> continents;
    std::string image;

    bool validate();
};

class Territory
{
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

class MapLoader
{

};

#endif //MAP_H