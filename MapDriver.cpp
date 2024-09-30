//
// Created by Dugua on 9/26/2024.
//

#include "MapDriver.h"
#include "Map.h"

void testLoadMaps() {
    MapLoader* mapLoader = new MapLoader();

    Map* map = mapLoader->readFile("USA.map");
    map->toString();
}

int main(int argc, char *argv[]) {
    testLoadMaps();
}