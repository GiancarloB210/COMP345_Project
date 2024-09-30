//
// Created by Dugua on 9/26/2024.
//

#ifndef MAP_H
#define MAP_H
#include <cstdint>
#include <iostream>
#include <sstream>
#include <list>
#include <fstream>

class Map;
class Continent;
class Territory;

std::ostream& operator << (std::ostream &os, Territory &territory);
std::ostream& operator << (std::ostream &os, Continent &continent);
std::ostream& operator << (std::ostream &os, Map &map);

class Territory
{
private:
    std::string name;
    int16_t xCoord;
    int16_t yCoord;
    std::list<std::string> adjacentTerritories;

public:
    Territory(std::string name, int16_t xCoord, int16_t yCoord, std::list<std::string> adjacentTerritories);
    Territory(Territory& territory);
    std::string getName();
    int16_t getXCoord();
    int16_t getYCoord();
    std::list<std::string> getAdjacentTerritories();
    std::string toString();
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
    std::string getName();
    int16_t getScore();
    std:: string toString();
};


class Map
{
private:
    std::string name;
    std::string image;
    bool isWrappable;
    bool scrollsVertically;
    std::string author;
    bool includeWarnings;
    std::list<Continent*> continents;

public:
    Map(std::string name, std::string image, bool isWrappable, bool scrollsVertically, std::list<Continent*> continents);
    Map(Map& map);

    std::string getName();
    std::string getImage();
    bool getIsWrappable();
    bool getScrollsVertically();
    std::string getAuthor();
    bool getIncludeWarnings();
    std::list<Continent*> getContinents();
    std:: string toString();

    bool validate();
};


class MapLoader
{
public:
    Map* readFile(std::string filePath);
};

#endif //MAP_H