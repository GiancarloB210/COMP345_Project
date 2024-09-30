//
// Created by Dugua on 9/26/2024.
//

#include "Map.h"

Territory::Territory(Territory &territory) {
    name = territory.name;
    xCoord = territory.xCoord;
    yCoord = territory.yCoord;
    adjacentTerritories = territory.adjacentTerritories;
}

Territory::Territory(std::string name, int16_t xCoord, int16_t yCoord, std::list<std::string> adjacentTerritories) {
    this->name = name;
    this->xCoord = xCoord;
    this->yCoord = yCoord;
    this->adjacentTerritories = adjacentTerritories;
}

std::string Territory::getName() {
    return this->name;
}

int16_t Territory::getXCoord() {
    return this->xCoord;
}

int16_t Territory::getYCoord() {
    return this->yCoord;
}

std::list<std::string> Territory::getAdjacentTerritories() {
    return this->adjacentTerritories;
}

 std::ostream& operator << (std::ostream &os, Territory &territory) {
    std::string adjacent;
    for (auto const& i : territory.getAdjacentTerritories()) {
        adjacent.append("\n\t " + i);
    }

    return (os << "Territory:\n" << "Name: " << territory.getName() << "\nXCoordinate: " << territory.getXCoord() +
        "\nYCoordinate: " << territory.getYCoord() << "\nadjacent territories:" << adjacent << "\n");
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

Continent::Continent(std::string name, int16_t score) {
    this->name = name;
    this->score = score;
}

std::list<Territory*> Continent::getTerritories() {
    return this->territories;
}

void Continent::setTerritories(std::list<Territory *> territories) {
    this->territories = territories;
}

std::string Continent::getName() {
    return this->name;
}

int16_t Continent::getScore() {
    return this->score;
}

std::ostream& operator << (std::ostream& os, Continent& continent) {
    std::string territories;
    for (auto const& i : continent.getTerritories()) {
        territories.append("\n\t" + i->toString());
    }
    return (os << "Country:\n" << "Name: " << continent.getName() << "\nScore: " << continent.getScore()
        << "\nTerritories:\n" << territories << "\n");
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

Map::Map(std::string name, std::string image, bool isWrappable, bool scrollsVertically, std::list<Continent *> continents) {
    this->name = name;
    this->image = image;
    this->isWrappable = isWrappable;
    this->scrollsVertically = scrollsVertically;
    this->author = author;
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
        continents.append("\n\t" + i->toString());
    }

    return (os << "Map:\n" << "Name: " << map.getName() << "\nImage: " << map.getImage()
        << "\nWraps?: " << map.getIsWrappable() << "\nScrolls Vertically?: " << map.getScrollsVertically()
        << "\nAuthor: " << map.getAuthor() << "\nInclude Warnings?: " << map.getIncludeWarnings()
        << "\nContinents:\n" << continents << "\n");
}

std::string Map::toString() {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}


bool Map::validate() {
    return true;
}

Map* MapLoader::readFile(std::string filePath) {
    std::string mapName = filePath.substr(0, filePath.find_last_of("."));
    std::string mapAuthor;
    bool mapIsWrappable;
    bool mapScrollsVertically;
    std::string mapImage;
    bool mapIncludeWarnings;
    std::list<Continent*> mapContinents;
    inputFileStream.exceptions(std::ifstream::badbit);

    try
    {
        std::cout << "Reading Map File: " << "MapFiles/" + filePath << std::endl;
        inputFileStream.open("MapFiles/" + filePath);

        while (!inputFileStream.eof()) {
            std::string line;
            std::getline(inputFileStream, line);

            if (!line.empty() && line.find("[Map]") != std::string::npos) {
                while(std::getline(inputFileStream, line)) {
                    if (line.empty())
                        break;

                    std::string delimiter = "=";
                    std::string token = line.substr(0, line.find(delimiter));
                    std::string tokenValue = line.substr(line.find(delimiter) + 1, line.length());

                    if (token == "author")
                            mapAuthor = tokenValue;
                    else if (token == "warn")
                    {
                        if (tokenValue == "true")
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
                    if (line.empty())
                        break;
                    std::string delimiter = "=";
                    std::string token = line.substr(0, line.find(delimiter));
                    std::string tokenValue = line.substr(line.find(delimiter) + 1, line.length());
                    int16_t tokenValueInt = std::stoi(tokenValue);

                    Continent* continent = new Continent(token, tokenValueInt);
                    mapContinents.push_back(continent);
                    delete continent;
                    continent = nullptr;
                }
            }

            if (!line.empty() && line.find("[Territories]") != std::string::npos) {
                std::string territoryName;
                int16_t territoryXCoord;
                int16_t territoryYCoord;
                std::list<std::string> adjacentTerritories;
                std::list<Territory*> continentTerritories;
                std::string countryName;

                while(!inputFileStream.eof()) {
                    std::list<std::string> tokens;
                    std:: string token;
                    size_t pos = 0;
                    std::string delimiter = ",";

                    while(std::getline(inputFileStream, line)) {
                        if (line.empty()) {
                            for (auto const& i : mapContinents) {
                                if (i->getName() == countryName)
                                    i->setTerritories(continentTerritories);
                            }
                            continue;
                        }

                        while ((pos = line.find(delimiter)) != std::string::npos) {
                            token = line.substr(0, pos);
                            tokens.push_back(token);
                            line.erase(0, pos + delimiter.length());
                        }
                        tokens.push_back(line);
                        std::string array[tokens.size()];
                        for (size_t i = 0; i < tokens.size(); i++) {
                            array[i] = tokens.front();
                            tokens.pop_front();
                        }

                        for(size_t i = 1; i < array->length(); i++) {
                            if (isdigit(array[i][0]) && i == 1) {
                                territoryName = array[i-1];
                                territoryXCoord = std::stoi(array[i]);
                                territoryYCoord = std::stoi(array[i+1]);
                                countryName = array[i+2];
                                i++;
                                i++;
                                continue;
                            }
                            if (isdigit(array[i][0])) {
                                continentTerritories.push_back(new Territory(territoryName, territoryXCoord,
                                                                             territoryYCoord, adjacentTerritories));
                                adjacentTerritories.clear();
                                territoryName = array[i-1];
                                territoryXCoord = std::stoi(array[i]);
                                territoryYCoord = std::stoi(array[i+1]);
                                i++;
                                i++;
                                continue;
                            }
                            adjacentTerritories.push_back(array[i]);
                        }
                    }
                }
            }
        }
    }
    catch (const ifstream::failure& e)
    {
    cout << "Error: " << e.what() << endl;
    }
    inputFileStream.close();
    return new Map(mapName,mapImage, mapIsWrappable, mapScrollsVertically, mapContinents);
}
