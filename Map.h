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

    Territory(std::string name, int16_t xCoord, int16_t yCoord, std::list<Territory*> adjacentTerritories);
    Territory(Territory& territory);
};

class Continent
{
    std::string name;
    int16_t score;
    std::list<Territory*> territories;

    Continent(std::string name, int16_t score, std::list<Territory*> territories);
    Continent(Continent& continent);
};

class Map
{
    std::string image;
    bool isWrappable;
    bool scrollsVertically;
    std::string author;
    bool includeWarnings;

    std::list<Continent*> continents;

    Map(std::string image, bool isWrappable, bool scrollsVertically, std::list<Continent*> continents);
    Map(Map& map);

    bool validate();
};

class MapLoader
{
    std::ifstream inputFileStream;
    bool readFile(std::string filePath);
};

#endif //MAP_H