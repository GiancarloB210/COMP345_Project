#include "Player.h"
#include <iostream>
#include <algorithm>

// Parameterized constructor (without name input)
Player::Player(std::vector<Territory*>* territories, Hand* hand) {
    this->playerID = staticPlayerID;
    staticPlayerID++;
    this->name = "No Name";
    this->territories = new std::vector<Territory*>(*territories);
    this->hand = new Hand(*hand);
    this->orders = new OrderList();
    this->armyUnits = std::vector<ArmyUnit*>();
    this->reinforcementPool = 0; // Initialize reinforcement pool
}

// Parameterized constructor (with name input)
Player::Player(std::string newName, std::vector<Territory*>* territories, Hand* hand) {
    this->playerID = staticPlayerID;
    staticPlayerID++;
    this->name = newName;
    this->territories = new std::vector<Territory*>(*territories);
    this->hand = new Hand(*hand);
    this->orders = new OrderList();
    this->armyUnits = std::vector<ArmyUnit*>();
    this->reinforcementPool = 0; // Initialize reinforcement pool
}

// Copy constructor (deep copy)
Player::Player(const Player& player) {
    this->playerID = staticPlayerID;
    staticPlayerID++;
    this->name = player.name;
    this->territories = new std::vector<Territory*>(*player.territories);
    this->hand = new Hand(*player.hand);
    this->orders = new OrderList(*player.orders);
    this->armyUnits = std::vector<ArmyUnit*>(player.armyUnits);
    this->reinforcementPool = player.reinforcementPool;
    this->currentGame = player.currentGame;
}

// Destructor to clean up dynamic memory
Player::~Player() {
    delete this->territories;
    this->territories = NULL;
    delete this->hand;
    this->hand = NULL;
    delete this->orders;
    this->orders = NULL;
    for (int i = 0; i < this->armyUnits.size(); i++) {
        delete this->armyUnits[i];
        this->armyUnits[i] = NULL;
    }
    this->armyUnits.clear();
}

// Assignment operator (deep copy)
Player& Player::operator=(const Player& player) {
    if (this != &player) {
        delete this->territories;
        delete this->hand;
        delete this->orders;
        this->armyUnits.clear();

        this->playerID = player.playerID;
        this->name = player.name;
        this->territories = new std::vector<Territory*>(*player.territories);
        this->hand = new Hand(*player.hand);
        this->orders = new OrderList(*player.orders);
        this->armyUnits = std::vector<ArmyUnit*>(player.armyUnits);
        this->reinforcementPool = player.reinforcementPool;
        this->currentGame = new GameEngine(*player.currentGame);
    }
    return *this;
}

// Stream insertion operator for printing player information
std::ostream& operator<<(std::ostream& out, const Player& player) {
    out << "Player's ID: " << player.playerID << std::endl;
    out << "Player's name: " << player.name << std::endl;
    out << "Player's territories to defend: ";
    for (Territory* t : *(player.territories)) {
        out << t->getName() << " ";
    }
    out << "\nPlayer's hand of cards: " << *(player.hand);
    out << "\nPlayer's list of orders: " << *(player.orders);
    out << "\nPlayer's number of army units: " << player.armyUnits.size();
    return out;
}

// Accessor methods
std::string Player::getName() const {
    return name;
}

const std::vector<Territory*>& Player::getTerritories() const {
    return *territories;
}

// Method to return a list of all territories which defend can be defended.
std::vector<Territory*> Player::toDefend() {
    return this->ps->toDefend();
}

std::vector<Territory*> Player::toAttack() {
    return this->ps->toAttack();
}

// Method to issue an order and add it to the player's list of orders
void Player::issueOrder() {
    this->ps->issueOrder();
}

void Player::playCard(int handIndex) {
    Order* returnedOrder = this->hand->play(handIndex);
    BombOrder* bo = dynamic_cast<BombOrder*>(returnedOrder);
    if (bo != NULL) {
        //List of candidate bomb targets.
        std::vector<Territory*> bombList;
        int iterator = 0;
        cout<<"List of territories that can be bombed by player "<<playerID<<endl;
        for (int i = 0;i < this->territories->size();i++) {
            for (int j = 0;j < (*this->territories)[i]->getAdjacentTerritories().size();j++) {
                //Only return an adjacent territory if it doesn't belong to the current player.
                if ((*this->territories)[i]->getAdjacentTerritories()[j]->getPlayer() != this) {
                    //Only add a territory to the list of potential bomb targets if it isn't already on the list.
                    if (std::find(bombList.begin(), bombList.end(), (*this->territories)[i]->getAdjacentTerritories()[j]) == bombList.end()) {
                        bombList.push_back((*this->territories)[i]->getAdjacentTerritories()[j]);
                        cout<<"["<<iterator<<"] "<<(*this->territories)[i]->getAdjacentTerritories()[j]->getName()<<endl;
                        iterator++;
                    }
                }
            }
        }
        //Index of enemy territory to be bombed
        int bombedIndex = 0;
        bool validBombOrder = false;
        while (!validBombOrder) {
            cout << "Enter the index corresponding to the opposing territory you wish to bomb."<<endl;
            cin >> bombedIndex;
            if (cin.fail() || (bombedIndex < 0 || bombedIndex > bombList.size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid number of army units." << endl;
            } else {
                validBombOrder = true;
                orders->add(new BombOrder(this, bombList[bombedIndex]));
            }
        }
        return;
    }
    //Reinforcement card played -> give current player 10 extra units.
    DeployOrder* d_order = dynamic_cast<DeployOrder*>(returnedOrder);
    if (d_order != NULL) {
        for (int i = 1;i < 10;i++) {
            this->armyUnits.push_back(new ArmyUnit(this));
        }
        this->reinforcementPool += 10;
        return;
    }
    BlockadeOrder* bl_order = dynamic_cast<BlockadeOrder*>(returnedOrder);
    if (bl_order != NULL) {
        //All owned territories can technically have a blockade set up.
        std::vector<Territory*> blockadeList = toDefend();
        cout<<"Enter the index corresponding to the territory which you would like to set up a blockade for."<<endl;
        cout<<"Note that ownership of the territory will transfer to a Neutral player."<<endl;
        //This 'iterator' variable isn't used, but it might be in a later code iteration.
        int iterator = 0;
        int blockadeIndex = 0;
        bool validBlockadeCheck = false;
        while (!validBlockadeCheck) {
            cout << "Enter the index of the territory which you would like to defend." << endl;
            cin >> blockadeIndex;
            if (cin.fail() || (blockadeIndex < 0 || blockadeIndex > blockadeList.size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid index." << endl;
            } else {
                validBlockadeCheck = true;
                orders->add(new BlockadeOrder(this, blockadeList[blockadeIndex]));
            }
        }
        return;
    }
    AirliftOrder* ao = dynamic_cast<AirliftOrder*>(returnedOrder);
    if (ao != NULL) {
        //Same as an advance order from one owned territory to another, but applying to a card
        //and its associated order type.
        std::vector<TempTerritoryUnitInfo> infoList = getTempTerritoryUnitInfo();
        int fromIndex_Airlift = 0;
            int toIndex_Airlift = 0;
            Territory* fromTerritory = NULL;
            Territory* toTerritory = NULL;
            int numUnitsOfFromTerritory = 0;
            int numUnitsSent = 0;
            bool validAirliftCommandCheck = false;
            bool validFromIndexCheck = false;
            bool validToIndexCheck = false;
            bool validUnitsSentCheck = false;
            while (!validAirliftCommandCheck) {
                cout<<"List of owned territories of player "<<playerID<<" with units in them:"<<endl;
                int indexer = 0;
                std::vector<Territory*> territoriesWithUnits;
                for (int i = 0;i < infoList.size();i++) {
                    if (infoList[i].numUnitsThere > 0) {
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout<<"["<<indexer<<"] "<<infoList[i].territory->getName()<<" (with "<<infoList[i].numUnitsThere<<" units)"<<endl;
                        indexer++;
                    }
                }
                while (!validFromIndexCheck) {
                    cout << "Enter the index of the territory which you would like to deploy units from" << endl;
                    cin >> fromIndex_Airlift;
                    if (cin.fail() || (fromIndex_Airlift < 0 || fromIndex_Airlift > territoriesWithUnits.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validFromIndexCheck = true;
                        fromTerritory = territoriesWithUnits[fromIndex_Airlift];
                    }
                }


                indexer = 0;
                std::vector<Territory*> adjTerritoriesToFrom;
                for (int i = 0;i < (*this->territories).size();i++) {
                    std::vector<Territory*> adjacentToCurrent = (*this->territories)[i]->getAdjacentTerritories();
                    //If any territory in the defend list is found in the adjacent territories list
                    if (std::find(adjacentToCurrent.begin(), adjacentToCurrent.end(), fromTerritory) != adjacentToCurrent.end()) {
                        //If the player owns both the territory where units are being sent from and the territory to defend, but the territories are not the same.
                        if (fromTerritory->getPlayer() == (*this->territories)[i]->getPlayer() && fromTerritory->getName() != (*this->territories)[i]->getName()) {
                            //If the territory is not already in the list.
                            if (std::find(adjTerritoriesToFrom.begin(), adjTerritoriesToFrom.end(), (*this->territories)[i]) == adjTerritoriesToFrom.end()) {
                                adjTerritoriesToFrom.push_back((*this->territories)[i]);
                                cout<<"["<<indexer<<"] "<<(*this->territories)[i]->getName()<<endl;
                                indexer++;
                            }
                        }
                    }
                }
                while (!validToIndexCheck) {
                    cout << "Enter the index of the territory which you would like to deploy units to" << endl;
                    cin >> toIndex_Airlift;
                    if (cin.fail() || (toIndex_Airlift < 0 || toIndex_Airlift > adjTerritoriesToFrom.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validToIndexCheck = true;
                        toTerritory = adjTerritoriesToFrom[toIndex_Airlift];
                    }
                }

                for (int i = 0;i < infoList.size();i++) {
                    if (infoList[i].territory->getName() == fromTerritory->getName()) {
                        numUnitsOfFromTerritory = infoList[i].numUnitsThere;
                    }
                }
                while (!validUnitsSentCheck) {
                    cout << "Enter the number of units you would like to send." << endl;
                    cin >> numUnitsSent;
                    if (cin.fail() || (numUnitsSent < 0 || numUnitsSent > numUnitsOfFromTerritory)) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validUnitsSentCheck = true;
                        validAirliftCommandCheck = true;
                        for (int j = 0;j < infoList.size();j++) {
                            if (infoList[j].territory->getName() == fromTerritory->getName()) {
                                infoList[j].numUnitsThere -= numUnitsSent;
                            }
                            if (infoList[j].territory->getName() == toTerritory->getName()) {
                                infoList[j].numUnitsThere += numUnitsSent;
                            }
                        }
                        cout<<"Deploying "<<numUnitsSent<<" from "<<fromTerritory->getName() << " to "<<toTerritory->getName()<<endl;
                        this->orders->add(new AirliftOrder(this, fromTerritory, toTerritory, numUnitsSent));
                    }
                }
            }
        return;
    }
    NegotiateOrder* no = dynamic_cast<NegotiateOrder*>(returnedOrder);
    if (no != NULL) {
        //This field is unused, but it may be used in a later code iteration.
        int indexer = 0;
        std::vector<int> playerIDs;
        //Gets all of the IDs from the player's associated game's player list and puts them all in a list.
        for (int i = 0;i < this->currentGame->getNumPlayers();i++) {
            playerIDs.push_back(i);
            cout<<"["<<i<<"] "<<this->currentGame->getPlayerByID(i)->getName()<<" with ID "<<this->currentGame->getPlayerByID(i)->playerID<<endl;
        }
        //Checks if a valid ID has been inputted;
        bool validIDCheck = false;
        //Chosen player ID corresponding to player to establish a negotiation with.
        int selectedID;
        while (!validIDCheck) {
            cout << "Enter the index of the territory which you would like to deploy units to" << endl;
            cin >> selectedID;
            if (cin.fail() || (selectedID < 0 || selectedID >= playerIDs.size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid index." << endl;
            } else {
                validIDCheck = true;
                //Gets the player from the selected ID, and establishes a negotiate order between the current player & that player.
                orders->add(new NegotiateOrder(this, this->currentGame->getPlayerByID(selectedID)));
            }
        }
    }
}

//Returns info of all owned terrtories and all associated army units for those territories.
//Associated: either in them already or about to be thanks to deployment orders.
std::vector<TempTerritoryUnitInfo> Player::getTempTerritoryUnitInfo() {
    std::vector<TempTerritoryUnitInfo> infoList;
    for (int i = 0;i < this->territories->size();i++) {
        //Will contain territory-unit info for the curret territory in the list.
        TempTerritoryUnitInfo newInfo;
        //Gets the number of army units already there.
        int deployedThere = (*this->territories)[i]->getArmyCount();
        //Gets the total number of to-be-deployed army units (thanks to DeployOrder conversions)
        //and adds this to the number of units assumed to be there.
        std::vector<Order*> orderList = this->orders->getList();
        for (int j = 0;j < orderList.size();j++) {
            if (dynamic_cast<DeployOrder*>(orderList[j]) != nullptr) {
                if (dynamic_cast<DeployOrder*>(orderList[j])->getTarget() == (*this->territories)[i]) {
                    deployedThere += dynamic_cast<DeployOrder*>(orderList[j])->getArmiesToAdd();
                }
            }
        }
        newInfo.territory = (*this->territories)[i];
        newInfo.numUnitsThere = deployedThere;
        infoList.push_back(newInfo);
    }
    return infoList;
}

// Adds units to the reinforcement pool
void Player::addToReinforcementPool(int units) {
    reinforcementPool += units;
}

// Checks if the player has more orders to issue or execute
bool Player::hasMoreOrders() const {
    return !orders->getList().empty();
}

// Checks if the player owns any territory
bool Player::ownsTerritory() const {
    return !territories->empty();
}

// Retrieves and removes the next order from the player's orders list
Order* Player::getNextOrder() {
    if (!orders->getList().empty()) {
        cout<<"Player "<<this->playerID<<" has "<<orders->getList().size()<<" order(s) left."<<endl;
        for (int i = 0;i < orders->getList().size();i++) {
            cout<<"["<<i<<"] "<<orders->getList()[i]->getType()<<endl;
        }
        Order* nextOrder = orders->getList().front();
        //Remove the first order from the list, as it is set to be returned and executed.
        std::vector<Order*> list = orders->getList();
        list.erase(list.begin());
        orders->setList(list);
        return nextOrder;
    }
    return nullptr; // Return nullptr if there are no orders
}
