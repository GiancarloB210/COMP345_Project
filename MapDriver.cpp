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

    std::cout << "\nValidating map USA" << std::endl;
    mapUSA->validate(mapUSA);

    std::cout << "\nValidating map USA_Fail1" << std::endl;
    mapUSAF1->validate(mapUSAF1);

    std::cout << "\nValidating map USA_Fail2" << std::endl;
    mapUSAF2->validate(mapUSAF2);

    std::cout << "\nValidating map USA_Fail3" << std::endl;
    mapUSAF3->validate(mapUSAF3);

    Map* map = mapLoader->readFile("map.txt");

    delete mapUSA;
    mapUSA = nullptr;
    delete mapUSAF1;
    mapUSAF1 = nullptr;
    delete mapUSAF2;
    mapUSAF2 = nullptr;
    delete mapUSAF3;
    mapUSAF3 = nullptr;
    delete mapLoader;
    mapLoader = nullptr;

}