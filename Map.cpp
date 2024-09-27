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

Continent::Continent(std::string name, int16_t score, std::list<Territory *> territories) {
    this->name = name;
    this->score = score;
    this->territories = territories;
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
    return true;
}
