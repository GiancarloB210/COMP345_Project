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

Territory::Territory(std::string name, int16_t xCoord, int16_t yCoord, std::list<Territory *> adjacentTerritories) {
    this->name = name;
    this->xCoord = xCoord;
    this->yCoord = yCoord;
    this->adjacentTerritories = adjacentTerritories;
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


Map::Map(Map &map) {
    image = map.image;
    isWrappable = map.isWrappable;
    scrollsVertically = map.scrollsVertically;
    author = map.author;
    continents = map.continents;
}

Map::Map(std::string image, bool isWrappable, bool scrollsVertically, std::list<Continent *> continents) {
    this->image = image;
    this->isWrappable = isWrappable;
    this->scrollsVertically = scrollsVertically;
    this->author = author;
    this->continents = continents;
}



bool Map::validate() {
    return true;
}

bool MapLoader::readFile(std::string filePath) {
    std::string mapAuthor;
    bool mapIsWrappable;
    bool mapScrollsVertically;
    std::string mapImage;
    bool mapIncludeWarnings;
    std::list<Continent*> mapContinents;

    inputFileStream.open(filePath);

    if (!inputFileStream.is_open()) {
        std::cout << "Error opening file " << filePath << std::endl;
        return false;
    }
    else {
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

                    switch (token) {
                        case "author": {
                            mapAuthor = tokenValue;
                        } break;
                        case "warn": {
                            if (tokenValue == "yes")
                                mapIncludeWarnings = true;
                            else
                                mapIncludeWarnings = false;
                        } break;
                        case "image": {
                            mapImage = tokenValue;
                        } break;
                        case "wrap": {
                            if (tokenValue == "yes")
                                mapIsWrappable = true;
                            else
                                mapIsWrappable = false;
                        } break;
                        case "scroll": {
                            if (tokenValue == "vertical")
                                mapScrollsVertically = true;
                            else
                                mapScrollsVertically = false;
                        } break;
                        default: break;
                    }
                }
            }

            if (!line.empty() && line.find("[Continents]") != std::string::npos) {
                std::string continentName;
                int16_t continentScore;
                std::list<Territory*> continentTerritories;

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
                std::list<Territory*> adjacentTerritories;

                while(!inputFileStream.eof()) {
                    std::list<std::string> tokens;
                    std:: string token;
                    size_t pos = 0;
                    std::string delimiter = ",";

                    while(std::getline(inputFileStream, line)) {
                        if (line.empty()) {
                            // TODO: Parse the Country's territories here
                        }

                        while ((pos = line.find(delimiter)) != std::string::npos) {
                            token = line.substr(0, pos);
                            tokens.push_back(token);
                            line.erase(0, pos + delimiter.length());
                        }
                    }
                }
            }
        }
    }
}
