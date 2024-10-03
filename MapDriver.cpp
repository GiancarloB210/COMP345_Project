//
// Created by Dugua on 9/26/2024.
//

#include "MapDriver.h"
#include "Map.h"

void testLoadMaps() {
    MapLoader* mapLoader = new MapLoader();

        Map* mapUSA = mapLoader->readFile("USA.map");
        std::cout << mapUSA->toString() << std::endl;

        Map* mapUSAF1 = mapLoader->readFile("USA_Fail1.map");
        std::cout << mapUSAF1->toString() << std::endl;

        Map* mapUSAF2 = mapLoader->readFile("USA_Fail2.map");
        std::cout << mapUSAF2->toString() << std::endl;

        Map* mapUSAF3 = mapLoader->readFile("USA_Fail3.map");
        std::cout << mapUSAF3->toString() << std::endl;

        Map* map = mapLoader->readFile("map.txt");


    std::cout << "\nValidating map USA" << std::endl;
    if (mapUSA->validate(mapUSA))
        std::cout << "Map '" + mapUSA->getName() + "' validation successful" << std::endl;
    else
        std::cout << "Map '" + mapUSA->getName() + "' validation failed" << std::endl;


    std::cout << "\nValidating map USA_Fail1" << std::endl;
    if (mapUSAF1->validate(mapUSAF1))
        std::cout << "Map '" + mapUSAF1->getName() + "' validation successful" << std::endl;
    else
        std::cout << "Map '" + mapUSAF1->getName() + "' validation failed" << std::endl;

    std::cout << "\nValidating map USA_Fail2" << std::endl;
    if (mapUSAF2->validate(mapUSAF2))
        std::cout << "Map '" + mapUSAF2->getName() + "' validation successful" << std::endl;
    else
        std::cout << "Map '" + mapUSAF2->getName() + "' validation failed" << std::endl;

    std::cout << "\nValidating map USA_Fail3" << std::endl;
    if (mapUSAF3->validate(mapUSAF3))
        std::cout << "Map '" + mapUSAF3->getName() + "' validation successful" << std::endl;
    else
        std::cout << "Map '" + mapUSAF3->getName() + "' validation failed\n" << std::endl;

    delete mapLoader;
    mapLoader = nullptr;
}