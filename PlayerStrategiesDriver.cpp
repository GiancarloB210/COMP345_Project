#include "PlayerStrategiesDriver.h"
#include "GameEngine.h"

void testPlayerStrategies() {
    MapLoader mapLoader;
    Map* map = mapLoader.readFile("../MapFiles/USA.map");
    GameEngine game;
    game.loadMap();
    game.validateMap();
    game.setUpPlayers();
    Player* p1 = game.getPlayerByID(0);
    Player* p2 = game.getPlayerByID(1);
    cout << "The game has been started." << endl;
    cout<<"---------------Aggressive-Benevolent------------------"<<endl;

    // Fairly distribute all the territories to the players
    game.distributeTerritories();

    // Randomly determine the order of play
    game.determinePlayerOrder();

    // Give 50 initial army units to each player
    game.allocateInitialArmies();

    // Let each player draw 2 initial cards from the deck
    game.drawInitialCards();

    game.reinforcementPhase();   // Assign reinforcements automatically
    game.issueOrdersPhase();     // Players issue orders in a round-robin fashion
    game.executeOrdersPhase();   // Execute orders in a round-robin fashion

    PlayerStrategy* ben_ps_p1 = new BenevolentPlayerStrategy(p1);
    PlayerStrategy* ben_ps_p2 = new BenevolentPlayerStrategy(p2);
    PlayerStrategy* agg_ps_p1 = new AggressivePlayerStrategy(p1);
    PlayerStrategy* agg_ps_p2 = new AggressivePlayerStrategy(p2);

    p1->ps = ben_ps_p1;
    p2->ps = agg_ps_p2;

    cout<<"---------------Benevolent-Aggressive------------------"<<endl;
    game.reinforcementPhase();   // Assign reinforcements automatically
    game.issueOrdersPhase();     // Players issue orders in a round-robin fashion
    game.executeOrdersPhase();   // Execute orders in a round-robin fashion

    p1->ps = agg_ps_p1;
    p2->ps = agg_ps_p2;

    cout<<"---------------Aggressive-Aggressive------------------"<<endl;
    game.reinforcementPhase();   // Assign reinforcements automatically
    game.issueOrdersPhase();     // Players issue orders in a round-robin fashion
    game.executeOrdersPhase();   // Execute orders in a round-robin fashion

    p1->ps = ben_ps_p1;
    p2->ps = ben_ps_p2;

    cout<<"---------------Benevolent-Benevolent------------------"<<endl;
    game.reinforcementPhase();   // Assign reinforcements automatically
    game.issueOrdersPhase();     // Players issue orders in a round-robin fashion
    game.executeOrdersPhase();   // Execute orders in a round-robin fashion
}