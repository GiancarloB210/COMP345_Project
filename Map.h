//
// Created by Dugua on 9/26/2024.
//

#ifndef MAP_H
#define MAP_H
#include <list>
#include <fstream>
#include <vector>
#include "Player.h"
#include <sstream>

// Forward Declarations
class Map;
class Continent;
class Territory;
class Player;

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
    std::vector<std::string> adjacentTerritoryNames;
    std::vector<Territory*> adjacentTerritories;
    Player* territoryOwner;
    int armyCount;

public:
    // Constructors
    Territory(std::string name, std::string continentName, int xCoord, int yCoord,
        std::vector<std::string> adjacentTerritoryNames);
    Territory(Territory& territory);

    // Operators
    Territory& operator=(const Territory& territory);

    // Deconstructor
    ~Territory();

    // Accessors
    std::string getName();
    std::string getContinentName();
    int getXCoord();
    int getYCoord();
    int getArmyCount();
    std::vector<std::string> getAdjacentTerritoryNames();
    std::vector<Territory*> getAdjacentTerritories();

    // Mutators
    void setAdjacentTerritories(std::vector<Territory*> adjacentTerritories);
    void addAdjacentTerritory(Territory* territory);
    void setArmyCount(int armyCount);
    Player* getPlayer();
    void setPlayer(Player* player);

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

    // Operators
    Continent& operator=(const Continent& continent);

    // Deconstructor
    ~Continent();

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

    // Operators
    Map& operator=(const Map& map);

    // Deconstructor
    ~Map();

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
    bool traverseTerritories(std::vector<Territory*> adjacentTerritories, std::list<Territory*> &uncheckedTerritories,
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

    // Operators
    MapLoader& operator=(const MapLoader& mapLoader);

    // Deconstructor
    ~MapLoader();

    // Accessors
    std::list<Map*> getMaps();

    // Mutators
    void addMap(Map* map);

    // Methods
    std::vector<std::string> split(std::string& s, std::string& delimeter);
    Map* readFile(std::string filePath);
};

#endif //MAP_H