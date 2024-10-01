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
#include <vector>
#include <set>

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
    std::string continentName;
    int xCoord;
    int yCoord;
    std::list<std::string> adjacentTerritoryNames;
    std::list<Territory*> adjacentTerritories;

public:
    Territory(std::string name, std::string continentName, int xCoord, int yCoord,
        std::list<std::string> adjacentTerritoryNames);
    Territory(Territory& territory);
    std::string getName();
    std::string getContinentName();
    int getXCoord();
    int getYCoord();
    std::list<std::string> getAdjacentTerritoryNames();
    std::list<Territory*> getAdjacentTerritories();
    void setAdjacentTerritories(std::list<Territory*> adjacentTerritories);
    void addAdjacentTerritory(Territory* territory);
    std::string toString();
};


class Continent
{
private:
    std::string name;
    int score;
    std::list<Territory*> territories;
    std::list<Continent*> adjacentContinents;

public:
    Continent(std::string name, int score);
    Continent(Continent& continent);

    std::list<Territory*> getTerritories();
    void setTerritories(std::list<Territory*> territories);
    std::list<Continent*> getAdjacentContinents();
    void setAdjacentContinents(std::list<Continent*> adjacentContinents);
    void addAdjacentContinent(Continent* continent);
    std::string getName();
    int getScore();
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
    Map(std::string name, std::string image, bool isWrappable, bool scrollsVertically, std::string author,
        bool includeWarnings, std::list<Continent*> continents);
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