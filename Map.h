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
private:
    std::string name;
    int16_t xCoord;
    int16_t yCoord;
    std::list<Territory*> adjacentTerritories;

public:
    Territory(std::string name, int16_t xCoord, int16_t yCoord, std::list<Territory*> adjacentTerritories);
    Territory(Territory& territory);
};

class Continent
{
private:
    std::string name;
    int16_t score;
    std::list<Territory*> territories;

public:
    Continent(std::string name, int16_t score);
    Continent(Continent& continent);

    std::list<Territory*> getTerritories();
    void setTerritories(std::list<Territory*> territories);
};

class Map
{
private:
    std::string image;
    bool isWrappable;
    bool scrollsVertically;
    std::string author;
    bool includeWarnings;
    std::list<Continent*> continents;

public:
    Map(std::string image, bool isWrappable, bool scrollsVertically, std::list<Continent*> continents);
    Map(Map& map);

    bool validate();
};

class MapLoader
{
private:
    std::ifstream inputFileStream;

public:
    bool readFile(std::string filePath);
};

#endif //MAP_H