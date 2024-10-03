//
// Created by Dugua on 9/26/2024.
//

#include "Map.h"

// Territory Class Copy Constructor
Territory::Territory(Territory &territory) {
    name = territory.name;
    continentName = territory.continentName;
    xCoord = territory.xCoord;
    yCoord = territory.yCoord;
    adjacentTerritoryNames = territory.adjacentTerritoryNames;
    territoryOwner = territory.territoryOwner;
    armyCount = territory.armyCount;
}

// Territory Class Constructor
Territory::Territory(std::string name, std::string continentName, int xCoord, int yCoord,
    std::list<std::string> adjacentTerritoryNames) {
    this->name = name;
    this->continentName = continentName;
    this->xCoord = xCoord;
    this->yCoord = yCoord;
    this->adjacentTerritoryNames = adjacentTerritoryNames;
    this->territoryOwner = nullptr;
    this->armyCount = 0;
}

// Territory Class Deconstructor
Territory::~Territory() {
    for (auto const& i : adjacentTerritories) {
    delete i;
    }

    delete territoryOwner;
}

// Territory Class Accessors
std::string Territory::getName() {
    return this->name;
}

std::string Territory::getContinentName() {
    return this->continentName;
}

int Territory::getXCoord() {
    return this->xCoord;
}

int Territory::getYCoord() {
    return this->yCoord;
}

int Territory::getArmyCount() {
    return this->armyCount;
}

std::list<std::string> Territory::getAdjacentTerritoryNames() {
    return this->adjacentTerritoryNames;
}

std::list<Territory*> Territory::getAdjacentTerritories() {
    return this-> adjacentTerritories;
}

// Territory Class Mutators
void Territory::setAdjacentTerritories(std::list<Territory *> adjacentTerritories) {
    this->adjacentTerritories = adjacentTerritories;
}

void Territory::addAdjacentTerritory(Territory *territory) {
    this->adjacentTerritories.push_back(territory);
}

void Territory::setArmyCount(int armyCount) {
    this->armyCount = armyCount;
}


 Player* Territory::getPlayer() {
 return this->territoryOwner;
 }

 void Territory::setPlayer(Player* player) {
 this->territoryOwner = player;
 }

// Territory Class Operators
 std::ostream& operator << (std::ostream &os, Territory &territory) {
    std::string adjacent;
    for (auto const& i : territory.getAdjacentTerritories()) {
        adjacent.append("\n\t" + i->getName());
    }

    return (os << "Territory:\n" << "Name: " << territory.getName() << "\nContinent Name: " <<
        territory.getContinentName() << "\nXCoordinate: " << std::to_string(territory.getXCoord()) << "\nYCoordinate: "
        << std::to_string(territory.getYCoord()) << "\nadjacent territories:" << adjacent <<
         "\nTerritory Owner: " << territory.getPlayer() << "\nArmy Count: " << territory.getArmyCount() << "\n");
}

Territory& Territory::operator=(const Territory &territory) = default;

// Territory Class Methods
// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// Territory's data member values
std::string Territory::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}


// Continent Class Copy Constructor
Continent::Continent(Continent &continent) {
    name = continent.name;
    score = continent.score;
    territories = continent.territories;
}

// Continent Class Constructor
Continent::Continent(std::string name, int score) {
    this->name = name;
    this->score = score;
}

// Continent Class Deconstructor
Continent::~Continent() {
    for (auto const& i : territories) {
    delete i;
    }

    for (auto const& i : adjacentContinents) {
    delete i;
    }
}

// Continent Class Accessors
std::list<Territory*> Continent::getTerritories() {
    return this->territories;
}

std::list<Continent *> Continent::getAdjacentContinents() {
    return this->adjacentContinents;
}

std::string Continent::getName() {
    return this->name;
}

int Continent::getScore() {
    return this->score;
}

// Continent Class Mutators
void Continent::setAdjacentContinents(std::list<Continent *> adjacentContinents) {
    this->adjacentContinents = adjacentContinents;
}

void Continent::setTerritories(std::list<Territory *> territories) {
    this->territories = territories;
}

void Continent::addAdjacentContinent(Continent *continent) {
    this->adjacentContinents.push_back(continent);
}

// Continent Class Operators
std::ostream& operator << (std::ostream& os, Continent& continent) {
    std::string territories;
    std::string adjacent;
    for (auto const& i : continent.getTerritories()) {
        territories.append("\n" + i->toString());
    }

    for (auto const& i : continent.getAdjacentContinents()) {
        adjacent.append("\n\t" + i->getName());
    }
    return (os << "Continent:\n" << "Name: " << continent.getName() << "\nScore: " << std::to_string(continent.getScore())
        << "\nAdjacent Continents: " << adjacent << "\nTerritories:\n{" << territories << "}\n");
}

Continent& Continent::operator=(const Continent &continent) = default;

// Continent Class Methods
// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// Continent's data member values
std::string Continent::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}

// Map Class Copy Constructor
Map::Map(Map &map) {
    name = map.name;
    image = map.image;
    isWrappable = map.isWrappable;
    scrollsVertically = map.scrollsVertically;
    author = map.author;
    continents = map.continents;
    territories = map.territories;
}

// Map Class Constructor
Map::Map(std::string name, std::string image, bool isWrappable,  bool scrollsVertically, std::string author,
    bool includeWarnings, std::list<Continent *> continents, std::list<Territory *> territories, bool areTerritoriesValid) {
    this->name = name;
    this->image = image;
    this->isWrappable = isWrappable;
    this->scrollsVertically = scrollsVertically;
    this->author = author;
    this->includeWarnings = includeWarnings;
    this->continents = continents;
    this->territories = territories;
    this->areTerritoriesValid = areTerritoriesValid;
}

// Map Class Deconstructor
Map::~Map() {
    for (auto const& i : continents) {
    delete i;
    }

    for (auto const& i : territories) {
    delete i;
    }
}

// Map Class Accessors
std::string Map::getName() {
    return this->name;
}

std::string Map::getImage() {
    return this->image;
}

bool Map::getIsWrappable() {
    return this->isWrappable;
}

bool Map::getScrollsVertically() {
    return this->scrollsVertically;
}

std::string Map::getAuthor() {
    return this->author;
}

bool Map::getIncludeWarnings() {
    return this->includeWarnings;
}

std::list<Continent *> Map::getContinents() {
    return this->continents;
}

std::list<Territory *> Map::getTerritories() {
    return this->territories;
}

bool Map::getAreTerritoriesValid() {
    return this->areTerritoriesValid;
}

// Map Class Operators
std::ostream& operator << (std::ostream& os, Map& map) {
    std::string continents;
    for (auto const& i : map.getContinents()) {
        continents.append("\n" + i->toString());
    }

    return (os << "\nMap:\n" << "Name: " << map.getName() << "\nImage: " << map.getImage()
        << "\nWraps?: " << map.getIsWrappable() << "\nScrolls Vertically?: " << map.getScrollsVertically()
        << "\nAuthor: " << map.getAuthor() << "\nInclude Warnings?: " << map.getIncludeWarnings()
        << "\n\nContinents:\n" << continents << "\n");
}

Map& Map::operator=(const Map &map) = default;

// Map Class Methods
// toString() uses the overloaded << operator to generate a formatted string containing the values of the
// Map's data member values
std::string Map::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}

// traverseContinents checks to see if the Map's Continents are a connected graph by visiting Continents and their
// adjacentContinent list, while keeping track of a list of continents that haven't been visited yet
bool Map::traverseContinents(std::list<Continent*> adjacentContinents, std::list<Continent*> &uncheckedContinents,
    Continent* startingContinent) {

    for (auto const& i : uncheckedContinents) {
        if (i->getName() == startingContinent->getName()) {
            uncheckedContinents.remove(startingContinent);
            break;
        }
    }

    for (auto const& i : adjacentContinents) {
        if (std::find_if(uncheckedContinents.begin(), uncheckedContinents.end(), [i](Continent* c)
            {return c-> getName() == i->getName();}) != uncheckedContinents.end()) {
            traverseContinents(i->getAdjacentContinents(), uncheckedContinents, i);
            }
    }
    // The continents are a connected graph if no continents are unvisited, so we return true if uncheckedContinents
    // is empty
    return uncheckedContinents.empty();
}

// traverseTerritories checks to see if the Map's Territories are a connected graph by visiting Territories and their
// adjacentTerritory list, while keeping track of a list of territories that haven't been visited yet
bool Map::traverseTerritories(std::list<Territory*> adjacentTerritories, std::list<Territory*> &uncheckedTerritories,
    Territory* startingTerritory) {

    for (auto const& i : uncheckedTerritories) {
        if (i->getName() == startingTerritory->getName()) {
            uncheckedTerritories.remove(startingTerritory);
            break;
        }
    }

    for (auto const& i : adjacentTerritories) {
        if (std::find_if(uncheckedTerritories.begin(), uncheckedTerritories.end(), [i](Territory* t)
            {return t-> getName() == i->getName();}) != uncheckedTerritories.end()) {
            traverseTerritories(i->getAdjacentTerritories(), uncheckedTerritories, i);
            }
    }
    // The territories are a connected graph if no territories are unvisited, so we return true if uncheckedTerritories
    // is empty
    return uncheckedTerritories.empty();
}

// validate checks if the passed Map instance is valid by checking if the continents and territories are connected
// graphs and if each territory belongs to only one continent. returns true if and only if all 3 checks pass
bool Map::validate(Map* map) {
    bool validContinentGraph = false;
    bool validTerritoriesGraph = false;
    bool validTerritories = false;

    // validate that the continents are a connected graph by validating that a path exists from all continents to all
    // other continents
    std::list<Continent*> uncheckedContinents(map->getContinents());
    std::cout << "validating continent graph: " << std::endl;
    validContinentGraph = traverseContinents(map->getContinents().front()->getAdjacentContinents(), uncheckedContinents,
        map->getContinents().front());
    std::cout << "\tContinent Graph Validation Result: " << validContinentGraph << "\n" << std::endl;

    // validate that the territories are a connected graph by validating that a path exists from all territories to all
    // other territories
    std::list<Territory*> uncheckedTerritories(map->getTerritories());
    std::cout << "validating territory graph: " << std::endl;
    validTerritoriesGraph = traverseTerritories(map->getTerritories().front()->getAdjacentTerritories(),
        uncheckedTerritories,map->getTerritories().front());
    std::cout << "\tTerritory Graph Validation Result: " << validTerritoriesGraph << "\n" << std::endl;

    // validate that each territory belongs to one and only one continent by returning the value of areTerritoriesValid
    std::cout << "validating uniqueness of territories: " << std::endl;
    validTerritories = map->getAreTerritoriesValid();
    std::cout << "\tTerritories Validation Result: " << validTerritories << "\n" << std::endl;

    return (validContinentGraph && validTerritoriesGraph && validTerritories);
}


// MapLoader Class Copy Constructor
MapLoader::MapLoader(MapLoader &mapLoader) {
    (loadedMaps = mapLoader.loadedMaps);
}

// MapLoader Class Default Constructor
MapLoader::MapLoader() = default;

// MapLoader Class Deconstructor
MapLoader::~MapLoader() {
    for (auto const& i : loadedMaps) {
    delete i;
    }
}

// MapLoader Class Accessors
std::list<Map*> MapLoader::getMaps() {
    return this->loadedMaps;
}

// MapLoader Class Mutators
void MapLoader::addMap(Map *map) {
    this->loadedMaps.push_back(map);
}

// MapLoader Operators
MapLoader& MapLoader::operator=(const MapLoader &mapLoader) = default;

// MapLoader Class Methods
// split is a helper method that splits a string into a vector of string tokens depending on a passed delimiter
// used by the readFile method to parse .map files line by line
std::vector<std::string> MapLoader::split(std::string& s, std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

// readFile takes in a filePath string to a .map file and parses it line by line to create an instance of Map
Map *MapLoader::readFile(std::string filePath) {
    std::ifstream inputFileStream;
    inputFileStream.open(filePath);
    // Check if the file at the provided filePath can be opened
    if (!inputFileStream.is_open()) {
        std::cerr << "Error opening file " << filePath << std::endl;
        return nullptr;
    }

    std::string mapName = filePath.substr(filePath.find_last_of('\\')+1, filePath.find_last_of("."));
    std::string mapAuthor;
    bool mapIsWrappable;
    bool mapScrollsVertically;
    std::string mapImage;
    bool mapIncludeWarnings;
    std::list<Continent*> mapContinents;
    std::list<Territory*> mapTerritories;
    std::string line;

    bool mapTerritoriesAreValid = true;

    // Parse the file line by line
    while (std::getline(inputFileStream, line)) {
        // Look for the [Map] section of the file
        if (!line.empty() && line.find("[Map]") != std::string::npos) {
            // Once inside the [Map] section, extract the map data member values for this file
            while(std::getline(inputFileStream, line)) {
                // If a blank line is reached, we proceed to the next section of the file by breaking out of this loop
                if (line.empty())
                    break;

                std::string delimiter = "=";
                std::string token = line.substr(0, line.find(delimiter));
                std::string tokenValue = line.substr(line.find(delimiter) + 1, line.length());

                if (token == "author")
                    mapAuthor = tokenValue;
                else if (token == "warn")
                {
                    if (tokenValue == "yes")
                        mapIncludeWarnings = true;
                    else
                        mapIncludeWarnings = false;
                }
                else if (token == "image")
                    mapImage = tokenValue;
                else if (token == "wrap")
                {
                    if (tokenValue == "yes")
                        mapIsWrappable = true;
                    else
                        mapIsWrappable = false;
                }
                else if (token == "scroll")
                {
                    if (tokenValue == "vertical")
                        mapScrollsVertically = true;
                    else
                        mapScrollsVertically = false;
                }
            }
        }
        // Look for the [Continents] section of the file
        if (!line.empty() && line.find("[Continents]") != std::string::npos) {
            // Once inside the [Continents] section, extract the continent data members
            // and populate instances of Continent
            while(std::getline(inputFileStream, line)) {
                // if we reach a blank line, proceed to the next section by breaking out of the loop
                if (line.empty())
                    break;
                std::string delimiter = "=";
                std::string token = line.substr(0, line.find(delimiter));
                std::string tokenValue = line.substr(line.find(delimiter) + 1, line.length());
                int tokenValueInt = std::stoi(tokenValue);
                // We also store the continents in a general list within the Map instance
                mapContinents.push_back(new Continent(token, tokenValueInt));
            }
        }

        // Look for the [Territories] section of the file
        if (!line.empty() && line.find("[Territories]") != std::string::npos) {
            // Once inside the [Territories] section, extract the territory data members
            // and populate instances of Territory
            std::string territoryName;
            int territoryXCoord;
            int territoryYCoord;
            std::list<std::string> adjacentTerritoryNames;
            std::list<Territory*> continentTerritories;
            std::string continentName;

            while(!inputFileStream.eof()) {
                std::vector<std::string> tokens;
                std:: string token;
                size_t pos = 0;
                std::string delimiter = ",";

                while(std::getline(inputFileStream, line)) {
                    if (line.empty()) {
                        for (auto const& i : mapContinents) {
                            if (i->getName() == continentName)
                                i->setTerritories(continentTerritories);
                        }
                        continentTerritories.clear();
                        continue;
                    }

                    tokens = split(line, delimiter);

                    for(int i = 0; i < tokens.size(); i++) {
                        if (i == 0) {
                            territoryName = tokens.at(i);
                        }
                        else if (i == 1) {
                            territoryXCoord = std::stoi(tokens.at(i));
                        }
                        else if (i == 2) {
                            territoryYCoord = std::stoi(tokens.at(i));
                        }
                        else if (i == 3) {
                            continentName = tokens.at(i);
                        }
                        else if ( i > 3) {
                            adjacentTerritoryNames.push_back(tokens.at(i));
                        }
                    }
                    // we also keep track of what continent the Territory belongs to, so that we can add the list
                    // into the appropriate continent later
                    continentTerritories.push_back(new Territory(territoryName, continentName,  territoryXCoord,
                                                                     territoryYCoord, adjacentTerritoryNames));

                    // If this is a new territory, add it to mapTerritories
                    std::string currentTerritoryName = continentTerritories.back()->getName();
                    if (std::find_if(mapTerritories.begin(), mapTerritories.end(),
                        [&currentTerritoryName](Territory* t){return t->getName() == currentTerritoryName;})
                        == mapTerritories.end())
                        mapTerritories.push_back(continentTerritories.back());
                    // Else mark the map as invalid (duplicate territory)
                    else
                        mapTerritoriesAreValid = false;
                    adjacentTerritoryNames.clear();
                }
            }
            // We loop through the list of continents we created and update them with their list of territories
            for (auto const& i : mapContinents) {
                if (i->getName() == continentName)
                    i->setTerritories(continentTerritories);
            }
        }
    }
    // We're done reading from the file, so we close it
    inputFileStream.close();

    // Use the newly created list to create the adjacency list for each territory in each continent
    for (auto const& i : mapContinents) {
        for (auto const& j : i->getTerritories()) {
            for (auto const& k : j->getAdjacentTerritoryNames()) {
                for (auto const& l : mapTerritories)
                if (k == l->getName()) {
                    j->addAdjacentTerritory(l);
                }
            }
        }
    }

    std::list<Continent*> adjacentContinents;

    // Use the newly created territory adjacency list to create the adjacency list for each continent
    for ( auto const& i : mapContinents) {
        for (auto const& j : i->getTerritories()) {
            for (auto const& k : j->getAdjacentTerritories()) {
                if (k->getContinentName() != i->getName()) {
                    for (auto const& l : mapContinents) {
                        if (k->getContinentName() == l->getName()) {
                            adjacentContinents.push_back(l);
                        }
                    }
                }
            }
        }
        adjacentContinents.sort([](Continent* a, Continent* b) {return a->getName() < b->getName();});
        adjacentContinents.unique();
        i->setAdjacentContinents(adjacentContinents);
        adjacentContinents.clear();
    }
        // the map instance is created, populated, and returned
        return new Map(mapName,mapImage, mapIsWrappable, mapScrollsVertically, mapAuthor, mapIncludeWarnings,
            mapContinents, mapTerritories, mapTerritoriesAreValid);
}




