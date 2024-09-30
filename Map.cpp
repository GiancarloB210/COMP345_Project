//
// Created by Dugua on 9/26/2024.
//

#include "Map.h"

Territory::Territory(Territory &territory) {
    name = territory.name;
    continentName = territory.continentName;
    xCoord = territory.xCoord;
    yCoord = territory.yCoord;
    adjacentTerritoryNames = territory.adjacentTerritoryNames;
}

Territory::Territory(std::string name, std::string continentName, int xCoord, int yCoord,
    std::list<std::string> adjacentTerritoryNames) {
    this->name = name;
    this->continentName = continentName;
    this->xCoord = xCoord;
    this->yCoord = yCoord;
    this->adjacentTerritoryNames = adjacentTerritoryNames;
}

std::string Territory::getName() {
    return this->name;
}

int Territory::getXCoord() {
    return this->xCoord;
}

int Territory::getYCoord() {
    return this->yCoord;
}

std::list<std::string> Territory::getAdjacentTerritoryNames() {
    return this->adjacentTerritoryNames;
}

std::list<Territory*> Territory::getAdjacentTerritories() {
    return this-> adjacentTerritories;
}

void Territory::setAdjacentTerritories(std::list<Territory *> adjacentTerritories) {
    this->adjacentTerritories = adjacentTerritories;
}

void Territory::addAdjacentTerritory(Territory *territory) {
    this->adjacentTerritories.push_back(territory);
}

std::string Territory::getContinentName() {
    return this->continentName;
}

 std::ostream& operator << (std::ostream &os, Territory &territory) {
    std::string adjacent;
    for (auto const& i : territory.getAdjacentTerritories()) {
        adjacent.append("\n\t" + i->getName());
    }

    return (os << "Territory:\n" << "Name: " << territory.getName() << "\nContinent Name: " <<
        territory.getContinentName() << "\nXCoordinate: " << std::to_string(territory.getXCoord()) + "\nYCoordinate: "
        << std::to_string(territory.getYCoord()) << "\nadjacent territories:" << adjacent << "\n");
}

std::string Territory::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}

Continent::Continent(Continent &continent) {
    name = continent.name;
    score = continent.score;
    territories = continent.territories;
}

Continent::Continent(std::string name, int score) {
    this->name = name;
    this->score = score;
}

std::list<Territory*> Continent::getTerritories() {
    return this->territories;
}

void Continent::setTerritories(std::list<Territory *> territories) {
    this->territories = territories;
}

std::list<Continent *> Continent::getAdjacentContinents() {
    return this->adjacentContinents;
}

void Continent::setAdjacentContinents(std::list<Continent *> adjacentContinents) {
    this->adjacentContinents = adjacentContinents;
}


void Continent::addAdjacentContinent(Continent *continent) {
    this->adjacentContinents.push_back(continent);
}



std::string Continent::getName() {
    return this->name;
}

int Continent::getScore() {
    return this->score;
}

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

std::string Continent::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}

Map::Map(Map &map) {
    name = map.name;
    image = map.image;
    isWrappable = map.isWrappable;
    scrollsVertically = map.scrollsVertically;
    author = map.author;
    continents = map.continents;
}

Map::Map(std::string name, std::string image, bool isWrappable,  bool scrollsVertically, std::string author,
    bool includeWarnings, std::list<Continent *> continents) {
    this->name = name;
    this->image = image;
    this->isWrappable = isWrappable;
    this->scrollsVertically = scrollsVertically;
    this->author = author;
    this->includeWarnings = includeWarnings;
    this->continents = continents;
}

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

std::string Map::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}


bool Map::validate() {
    return true;
}

std::vector<std::string> split(std::string& s, std::string& delimiter) {
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

Map* MapLoader::readFile(std::string filePath) {
    std::string mapName = filePath.substr(filePath.find_last_of('\\')+1, filePath.find_last_of("."));
    std::string mapAuthor;
    bool mapIsWrappable;
    bool mapScrollsVertically;
    std::string mapImage;
    bool mapIncludeWarnings;
    std::list<Continent*> mapContinents;
    std::string line;
    std::ifstream inputFileStream(filePath);
    while (std::getline(inputFileStream, line)) {
        //std::cout << line << std::endl;

        if (!line.empty() && line.find("[Map]") != std::string::npos) {
            while(std::getline(inputFileStream, line)) {
                //std::cout << line << std::endl;
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

        if (!line.empty() && line.find("[Continents]") != std::string::npos) {
            while(std::getline(inputFileStream, line)) {
                //std::cout << line << std::endl;
                if (line.empty())
                    break;
                std::string delimiter = "=";
                std::string token = line.substr(0, line.find(delimiter));
                std::string tokenValue = line.substr(line.find(delimiter) + 1, line.length());
                int tokenValueInt = std::stoi(tokenValue);

                mapContinents.push_back(new Continent(token, tokenValueInt));
            }
        }

        if (!line.empty() && line.find("[Territories]") != std::string::npos) {
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
                    //std::cout << line << std::endl;
                    if (line.empty()) {
                        for (auto const& i : mapContinents) {
                            if (i->getName() == continentName)
                                i->setTerritories(continentTerritories);
                        }
                        continentTerritories.clear();
                        continue;
                    }

                    tokens = split(line, delimiter);

                    for(int i = 4; i < tokens.size(); i++) {
                        if (i == 4) {
                            territoryName = tokens.at(0);
                            territoryXCoord = std::stoi(tokens.at(1));
                            territoryYCoord = std::stoi(tokens.at(2));
                            continentName = tokens.at(3);

                        }
                        adjacentTerritoryNames.push_back(tokens.at(i));
                    }

                    //std::cout << territoryName << std::endl;
                    //std::cout << territoryXCoord << std::endl;
                    //std::cout << territoryYCoord << std::endl;
                    //std::cout << continentName << std::endl;

                    continentTerritories.push_back(new Territory(territoryName, continentName,  territoryXCoord,
                                                                     territoryYCoord, adjacentTerritoryNames));
                    adjacentTerritoryNames.clear();
                }
            }

            for (auto const& i : mapContinents) {
                if (i->getName() == continentName)
                    i->setTerritories(continentTerritories);
            }
        }
    }
    inputFileStream.close();
    std::list<Territory*> mapTerritories;

    // Create a unique list of all territories on the map
    for (auto const& i : mapContinents) {
        for (auto const& j : i->getTerritories()) {
            mapTerritories.push_back(j);
        }
    }
    mapTerritories.unique();

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

    // Use the newly created adjacency lists to create the adjacency list for each continent
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


        return new Map(mapName,mapImage, mapIsWrappable, mapScrollsVertically, mapAuthor, mapIncludeWarnings,
            mapContinents);
}




