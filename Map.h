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
#include <algorithm>

// Forward Declarations
class Map;
class Continent;
class Territory;

// Free functions
std::ostream& operator << (std::ostream &os, Territory &territory);
std::ostream& operator << (std::ostream &os, Continent &continent);
std::ostream& operator << (std::ostream &os, Map &map);

class Territory
{
private:
    // Data Members
    std::string name;
    std::string continentName;
    int xCoord;
    int yCoord;
    std::list<std::string> adjacentTerritoryNames;
    std::list<Territory*> adjacentTerritories;
    //TODO: uncomment after merge
    // Player* territoryOwner;
    int armyCount;

public:
    // Constructors
    Territory(std::string name, std::string continentName, int xCoord, int yCoord,
        std::list<std::string> adjacentTerritoryNames);
    Territory(Territory& territory);

    // Accessors
    std::string getName();
    std::string getContinentName();
    int getXCoord();
    int getYCoord();
    int getArmyCount();
    std::list<std::string> getAdjacentTerritoryNames();
    std::list<Territory*> getAdjacentTerritories();

    // Mutators
    void setAdjacentTerritories(std::list<Territory*> adjacentTerritories);
    void addAdjacentTerritory(Territory* territory);
    void setArmyCount(int armyCount);
    //TODO: uncomment after merge
    // Player* getPlayer();
    // void setPlayer();

    // Methods
    std::string toString();
};


class Continent
{
private:
    // Data Members
    std::string name;
    int score;
    std::list<Territory*> territories;
    std::list<Continent*> adjacentContinents;

public:
    // Constructors
    Continent(std::string name, int score);
    Continent(Continent& continent);

    // Accessors
    std::list<Territory*> getTerritories();
    std::list<Continent*> getAdjacentContinents();
    std::string getName();
    int getScore();

    // Mutators
    void setTerritories(std::list<Territory*> territories);
    void setAdjacentContinents(std::list<Continent*> adjacentContinents);
    void addAdjacentContinent(Continent* continent);

    // Methods
    std:: string toString();
};

class Map
{
private:
    // Data Members
    std::string name;
    std::string image;
    bool isWrappable;
    bool scrollsVertically;
    std::string author;
    bool includeWarnings;
    std::list<Continent*> continents;
    std::list<Territory*> territories;
    bool areTerritoriesValid;

public:
    // Constructors
    Map(std::string name, std::string image, bool isWrappable, bool scrollsVertically, std::string author,
        bool includeWarnings, std::list<Continent*> continents, std::list<Territory*> territories, bool areTerritoriesValid);
    Map(Map& map);

    // Accessors
    std::string getName();
    std::string getImage();
    bool getIsWrappable();
    bool getScrollsVertically();
    std::string getAuthor();
    bool getIncludeWarnings();
    std::list<Continent*> getContinents();
    std::list<Territory*> getTerritories();
    bool getAreTerritoriesValid();

    // Methods
    std:: string toString();
    bool traverseContinents(std::list<Continent*> adjacentContinents, std::list<Continent*> &uncheckedContinents,
    Continent* startingContinent);
    bool traverseTerritories(std::list<Territory*> adjacentTerritories, std::list<Territory*> &uncheckedTerritories,
    Territory* startingTerritory);
    bool validate(Map* map);
};


class MapLoader
{
private:
    // Data Members
    std::list<Map*> loadedMaps;

public:
    // Constructors
    MapLoader();
    MapLoader(MapLoader& mapLoader);

    // Accessors
    std::list<Map*> getMaps();

    // Mutators
    void addMap(Map* map);

    // Methods
    std::vector<std::string> split(std::string& s, std::string& delimeter);
    Map* readFile(std::string filePath);
};

#endif //MAP_H