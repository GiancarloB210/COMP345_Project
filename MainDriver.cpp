//
// Created by Dugua on 10/3/2024.
//

#include "MainDriver.h"

#include <OrdersDriver.h>
#include <string.h>

#include "CardsDriver.h"
#include "MapDriver.h"
#include "PlayerDriver.h"
#include "GameEngineDriver.h"
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "Orders.h"

int main(int argc, char* argv[]) {
    cout << argc << endl;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-console") == 0) {
            if (i + 1 < argc) {
                cout << "argument: \'" << argv[i+1] << "\' will be ignored." << endl;
                cout << "proceeding with argument: \'" << argv[i] << "\'." << endl;
                break;
            }
        }
        else if (argv[i] == "-file") {
            if (i + 1 > argc) {
                cout << "forgot to include file name, enter it now." << endl;
                std::string fileName;
                cin >> fileName;
                break;
            } else if ( i+2 == argc ) {
                cout << "proceeding with command file: \'" << argv[i+1] << "\'." << endl;
                break;
            } else {
                cout << "entered too many arguments. File names should not include spaces." << endl;
                cout << "program will now terminate." << endl;
                exit(-1);
            }
        }
    }

    /*
    testLoadMaps();
    testPlayers();
    testOrdersLists();
    testCards();
    testGameStates();
    */

    return 0;
}
