#include "PlayerStrategies.h"
#include <cmath>

PlayerStrategy::PlayerStrategy(Player* pi) {
    this->pi = pi;
}

HumanPlayerStrategy::HumanPlayerStrategy(Player* pi) : PlayerStrategy(pi){
    this->pi = pi;
}

void HumanPlayerStrategy::issueOrder() {
    //Number of units remaining after each order issuing.
    int numUnitsRemaining = this->pi->reinforcementPool;

    //List of territories to defend, organized by priority.
    std::vector<Territory*> defendList = toDefend();

    //List of territories to attack, organized by priority.
    std::vector<Territory*> attackList = toAttack();

    int defendIndex = 0;
    bool validTerritorySelectedCheck = false;

    int armiesSent = 0;
    bool validArmyNumberCheck = false;
    // Prevents the user from inputting a non-numerical input or invalid index.
    while (numUnitsRemaining > 0) {
        cout<<"Army units left for distribution: "<<numUnitsRemaining<<endl;
        defendIndex = 0;
        validTerritorySelectedCheck = false;
        while (!validTerritorySelectedCheck) {
            cout << "Enter the index of the territory which you would like to defend." << endl;
            cin >> defendIndex;
            if (cin.fail() || (defendIndex < 0 || defendIndex > (*this->pi->territories).size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid index." << endl;
            } else {
                validTerritorySelectedCheck = true;
            }
        }

        armiesSent = 0;
        validArmyNumberCheck = false;
        while (!validArmyNumberCheck) {
            cout << "Enter the number of army units you would like to deploy to " << defendList[defendIndex]->getName()<<endl;
            cin >> armiesSent;
            if (cin.fail() || (armiesSent < 0 || armiesSent > numUnitsRemaining)) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid number of army units." << endl;
            } else {
                validArmyNumberCheck = true;
                numUnitsRemaining -= armiesSent;
                this->pi->orders->add(new DeployOrder(this->pi, defendList[defendIndex], armiesSent));
            }
        }

        cout<<"List of territories to be defended (in order of highest priority): "<<endl;
        for (int i = 0;i < this->pi->orders->getList().size();i++) {
            cout<<"["<<i<<"] "<<(dynamic_cast<DeployOrder*>(this->pi->orders->getList()[i]))->getTarget()->getName()<< " with " << (dynamic_cast<DeployOrder*>(this->pi->orders->getList()[i]))->getArmiesToAdd() << " army units to be sent."<<endl;
        }
    }
    this->pi->reinforcementPool = 0;

    std::vector<TempTerritoryUnitInfo> infoList = this->pi->getTempTerritoryUnitInfo();

    // while (reinforcementPool > 0) {
    //     // Deploy all remaining reinforcement units to territories in toDefend list
    //     vector<Territory*> territoriesToDefend = toDefend();
    //     int unitsPerTerritory = reinforcementPool / territoriesToDefend.size();
    //     for (int i = 0;i < territoriesToDefend.size();i++) {
    //         orders->add(new DeployOrder(this, territoriesToDefend[i], unitsPerTerritory));
    //     }
    // }
    int continueAttackOrders = true;
    string continueResponse;
    bool continueResponseCheck = false;
    string attackOrDefend;
    bool attackOrDefendResponseCheck = false;
    while (continueAttackOrders) {
        while (!continueResponseCheck) {
            attackOrDefend = "";
            attackOrDefendResponseCheck = false;
            cout << "Would you like to deploy some of your units to other countries? [Yes/No]" << endl;
            cin >> continueResponse;
            if (cin.fail() || (continueResponse != "Yes" && continueResponse != "No")) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid response." << endl;
            } else {
                continueResponseCheck = true;
                if (continueResponse == "No") {
                    continueAttackOrders = false;
                }
            }
        }
        if (continueAttackOrders == false) {
            break;
        }
        while (!attackOrDefendResponseCheck) {
            cout << "Would you like to attack an opposing territory or defend one of yours? [Attack/Defend]" << endl;
            cin >> attackOrDefend;
            if (cin.fail() || (attackOrDefend != "Attack" && attackOrDefend != "Defend")) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid response." << endl;
            } else {
                attackOrDefendResponseCheck = true;
            }
        }

        if (attackOrDefend == "Defend") {
            int fromIndex_Defend = 0;
            int toIndex_Defend = 0;
            Territory* fromTerritory = NULL;
            Territory* toTerritory = NULL;
            int numUnitsOfFromTerritory = 0;
            int numUnitsSent = 0;
            bool validDefendCommandCheck = false;
            bool validFromIndexCheck = false;
            bool validToIndexCheck = false;
            bool validUnitsSentCheck = false;
            while (!validDefendCommandCheck) {
                cout<<"List of owned territories of player "<<this->pi->playerID<<" with units in them:"<<endl;
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
                    cin >> fromIndex_Defend;
                    if (cin.fail() || (fromIndex_Defend < 0 || fromIndex_Defend > territoriesWithUnits.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validFromIndexCheck = true;
                        fromTerritory = territoriesWithUnits[fromIndex_Defend];
                    }
                }


                indexer = 0;
                std::vector<Territory*> adjTerritoriesToFrom;
                for (int i = 0;i < defendList.size();i++) {
                    std::vector<Territory*> adjacentToCurrent = defendList[i]->getAdjacentTerritories();
                    //If any territory in the defend list is found in the adjacent territories list
                    if (std::find(adjacentToCurrent.begin(), adjacentToCurrent.end(), fromTerritory) != adjacentToCurrent.end()) {
                        //If the player owns both the territory where units are being sent from and the territory to defend, but the territories are not the same..
                        if (fromTerritory->getPlayer() == defendList[i]->getPlayer() && fromTerritory->getName() != defendList[i]->getName()) {
                            //If the territory is not already in the list.
                            if (std::find(adjTerritoriesToFrom.begin(), adjTerritoriesToFrom.end(), defendList[i]) == adjTerritoriesToFrom.end()) {
                                adjTerritoriesToFrom.push_back(defendList[i]);
                                cout<<"["<<indexer<<"] "<<defendList[i]->getName()<<endl;
                                indexer++;
                            }
                        }
                    }
                }
                while (!validToIndexCheck) {
                    cout << "Enter the index of the territory which you would like to deploy units to" << endl;
                    cin >> toIndex_Defend;
                    if (cin.fail() || (toIndex_Defend < 0 || toIndex_Defend > adjTerritoriesToFrom.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validToIndexCheck = true;
                        toTerritory = adjTerritoriesToFrom[toIndex_Defend];
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
                        for (int j = 0;j < infoList.size();j++) {
                            if (infoList[j].territory->getName() == fromTerritory->getName()) {
                                infoList[j].numUnitsThere -= numUnitsSent;
                            }
                            if (infoList[j].territory->getName() == toTerritory->getName()) {
                                infoList[j].numUnitsThere += numUnitsSent;
                            }
                        }
                        cout<<"Deploying "<<numUnitsSent<<" from "<<fromTerritory->getName() << " to "<<toTerritory->getName()<<endl;
                        this->pi->orders->add(new AdvanceOrder(this->pi, fromTerritory, toTerritory, numUnitsSent));
                    }
                }
                validDefendCommandCheck = true;
                continueResponseCheck = false;
                cout<<"List of owned territories of player "<<this->pi->playerID<<" with units in them:"<<endl;
                indexer = 0;
                for (int i = 0;i < infoList.size();i++) {
                    if (infoList[i].numUnitsThere > 0) {
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout<<"["<<indexer<<"] "<<infoList[i].territory->getName()<<" (with "<<infoList[i].numUnitsThere<<" units)"<<endl;
                        indexer++;
                    }
                }
            }
        }

        if (attackOrDefend == "Attack") {
            int fromIndex_Attack = 0;
            int toIndex_Attack = 0;
            Territory* fromTerritory = NULL;
            Territory* toTerritory = NULL;
            int numUnitsOfFromTerritory = 0;
            int numUnitsSent = 0;
            bool validAttackCommandCheck = false;
            bool validFromIndexCheck = false;
            bool validToIndexCheck = false;
            bool validUnitsSentCheck = false;
            while (!validAttackCommandCheck) {
                cout<<"List of owned territories of player "<<this->pi->playerID<<" with units in them:"<<endl;
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
                    cin >> fromIndex_Attack;
                    if (cin.fail() || (fromIndex_Attack < 0 || fromIndex_Attack > territoriesWithUnits.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validFromIndexCheck = true;
                        fromTerritory = territoriesWithUnits[fromIndex_Attack];
                    }
                }
                indexer = 0;
                std::vector<Territory*> adjacentTerritoriesToAttack;
                for (int i = 0;i < attackList.size();i++) {
                    std::vector<Territory*> adjacentToCurrent = attackList[i]->getAdjacentTerritories();
                    if (std::find(adjacentToCurrent.begin(), adjacentToCurrent.end(), fromTerritory) != adjacentToCurrent.end()) {
                        if (fromTerritory->getPlayer() != attackList[i]->getPlayer()) {
                            if (std::find(adjacentTerritoriesToAttack.begin(), adjacentTerritoriesToAttack.end(), attackList[i]) == adjacentTerritoriesToAttack.end()) {
                                adjacentTerritoriesToAttack.push_back(attackList[i]);
                                cout<<"["<<indexer<<"] "<<attackList[i]->getName()<<endl;
                                indexer++;
                            }
                        }
                    }
                }
                while (!validToIndexCheck) {
                    cout << "Enter the index of the territory which you would like to deploy units to" << endl;
                    cin >> toIndex_Attack;
                    if (cin.fail() || (toIndex_Attack < 0 || toIndex_Attack > adjacentTerritoriesToAttack.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    } else {
                        validToIndexCheck = true;
                        toTerritory = adjacentTerritoriesToAttack[toIndex_Attack];
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
                        for (int j = 0;j < infoList.size();j++) {
                            if (infoList[j].territory->getName() == fromTerritory->getName()) {
                                infoList[j].numUnitsThere -= numUnitsSent;
                            }
                            if (infoList[j].territory->getName() == toTerritory->getName()) {
                                infoList[j].numUnitsThere += numUnitsSent;
                            }
                        }
                        cout<<"Deploying "<<numUnitsSent<<" from "<<fromTerritory->getName() << " to attack "<<toTerritory->getName()<<endl;
                        this->pi->orders->add(new AdvanceOrder(this->pi, fromTerritory, toTerritory, numUnitsSent));
                    }
                }
                validAttackCommandCheck = true;
                continueResponseCheck = false;
                cout<<"List of owned territories of player "<<this->pi->playerID<<" with units in them:"<<endl;
                indexer = 0;
                for (int i = 0;i < infoList.size();i++) {
                    if (infoList[i].numUnitsThere > 0) {
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout<<"["<<indexer<<"] "<<infoList[i].territory->getName()<<" (with "<<infoList[i].numUnitsThere<<" units)"<<endl;
                        indexer++;
                    }
                }
            }
        }
    }
    bool willPlayCard = false;
    string willPlayCardStr;
    bool validCardResponse = false;
    int cardIndex = 0;
    bool validCardIndexChosen = false;
    Card* chosenCard;
    while (!validCardResponse) {
        cout<<"Would you like to play a card from your hand? [Yes/No]"<<endl;
        cin>>willPlayCardStr;
        if (cin.fail() || (willPlayCardStr != "Yes" && willPlayCardStr != "No")) {
            cin.clear();
            cin.ignore();
            cout << "Invalid. Please enter a valid response." << endl;
        } else {
            validCardResponse = true;
            if (willPlayCardStr == "Yes") {
                willPlayCard = true;
            }
            if (willPlayCardStr == "No") {
                willPlayCard = false;
            }
        }
    }
    if (willPlayCard) {
        validCardResponse = false;
        validCardIndexChosen = false;
        willPlayCardStr = "";
        cout<<"Cards in the hand of player "<<this->pi->playerID<<endl;
        int indexer = 0;
        for (int i = 0;i < this->pi->hand->cardsInHand.size();i++) {
            cout<<"["<<indexer<<"] "<<this->pi->hand->cardsInHand[i]->getCardTypeStringValue()<<endl;
            indexer++;
        }
        while (!validCardIndexChosen) {
            cout << "Enter the index of the card which you would like to play." << endl;
            cin >> cardIndex;
            if (cin.fail() || (cardIndex < 0 || cardIndex > this->pi->hand->cardsInHand.size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid index." << endl;
            } else {
                validCardIndexChosen = true;
                chosenCard = this->pi->hand->cardsInHand[cardIndex];
                this->pi->playCard(cardIndex);
            }
        }
    }
}

// Method to return a list of all territories which defend can be defended.
std::vector<Territory*> HumanPlayerStrategy::toDefend() {

    //List of territories which can be defended.
    std::vector<Territory*> defendList;

    cout<<"List of territories owned by player "<<this->pi->playerID<<" that can be defended:"<<endl;
    for (int i = 0;i < (*this->pi->territories).size();i++) {
        defendList.push_back((*this->pi->territories)[i]);
        cout<<"["<<i<<"] "<<(*this->pi->territories)[i]->getName()<<endl;
    }
    return defendList;
}

std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    for (int i = 0;i < (*this->pi->territories).size();i++) {
        for (int j = 0;j < (*this->pi->territories)[i]->getAdjacentTerritories().size();j++) {
            //Only return an adjacent territory if it doesn't belong to the current player.
            if ((*this->pi->territories)[i]->getAdjacentTerritories()[j]->getPlayer() != this->pi) {
                attackList.push_back((*this->pi->territories)[i]->getAdjacentTerritories()[j]);
            }
        }
    }
    return attackList;
}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player* pi) : PlayerStrategy(pi){
    this->pi = pi;
}

void AggressivePlayerStrategy::issueOrder() {
    cout<<"Aggressive Player order issue"<<endl;
    std::vector<Territory*> orderedByStrongest = this->toDefend();
    //The first territory in the returned list is the one with the most territories.
    Territory* strongest = orderedByStrongest[0];
    std::vector<Territory*> adjacentToAttack;
    std::vector<Territory*> adjacent = strongest->getAdjacentTerritories();
    for (int j = 0;j < adjacent.size();j++) {
        if (adjacent[j]->getPlayer() != this->pi) {
            adjacentToAttack.push_back(adjacent[j]);
        }
    }
    cout<<"adjacentToAttack initialized"<<endl;
    int armiesToDeploy = this->pi->reinforcementPool;
    this->pi->orders->add(new DeployOrder(this->pi, strongest, armiesToDeploy));
    cout<<"DeployOrder added"<<endl;
    this->pi->reinforcementPool = 0;
    int armiesPerEnemyTerritory = floor(armiesToDeploy / adjacentToAttack.size());
    armiesToDeploy -= armiesPerEnemyTerritory * adjacentToAttack.size();
    for (int i = 0;i < adjacentToAttack.size();i++) {
        this->pi->orders->add(new AdvanceOrder(this->pi, strongest, adjacentToAttack[i], armiesPerEnemyTerritory));
    }
    cout<<"AdvanceOrders added"<<endl;
    if (armiesToDeploy > 0 && adjacentToAttack.empty() == false) {
        this->pi->orders->add(new AdvanceOrder(this->pi, strongest, adjacentToAttack[0], armiesToDeploy));
    }
    cout<<"More AdvanceOrders added"<<endl;
    bool bombAdded = false;
    for (int i = 0;i < this->pi->hand->cardsInHand.size();i++) {
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "BOMB" && adjacentToAttack.empty() == false) {
            this->pi->orders->add(new BombOrder(this->pi, adjacentToAttack[i]));
            bombAdded = true;
        }
    }
    cout<<(bombAdded == true ? "BombOrder added" : "BombOrder not added")<<endl;
}

std::vector<Territory *> AggressivePlayerStrategy::toDefend() {
    //List of territories which can be defended.
    std::vector<Territory*> defendList;

    for (int i = 0;i < (*this->pi->territories).size();i++) {
        defendList.push_back((*this->pi->territories)[i]);
    }
    //Sorts by highest number of army units.
    auto armyComparator = [](Territory* t1, Territory* t2) { return t1->getArmyCount() >= t2->getArmyCount(); };
    std::sort(defendList.begin(), defendList.end(), armyComparator);
    return defendList;
}

std::vector<Territory *> AggressivePlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    for (int i = 0;i < (*this->pi->territories).size();i++) {
        //Only attack an adjacent territory if the current territory actually has armies in it.
        if ((*this->pi->territories)[i]->getArmyCount() > 0) {
            for (int j = 0;j < (*this->pi->territories)[i]->getAdjacentTerritories().size();j++) {
                //Only return an adjacent territory if it doesn't belong to the current player.
                if ((*this->pi->territories)[i]->getAdjacentTerritories()[j]->getPlayer() != this->pi) {
                    attackList.push_back((*this->pi->territories)[i]->getAdjacentTerritories()[j]);
                }
            }
        }
    }
    return attackList;
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* pi) : PlayerStrategy(pi){
    this->pi = pi;
}

void BenevolentPlayerStrategy::issueOrder() {
    std::vector<Territory*> orderedByWeakest = this->toDefend();
    std::vector<Territory*> allWeakest;
    allWeakest.push_back(orderedByWeakest[0]);
    int minNumArmies = orderedByWeakest[0]->getArmyCount();
    int indexer = 1;
    while (orderedByWeakest[indexer]->getArmyCount() < minNumArmies) {
        allWeakest.push_back(orderedByWeakest[indexer]);
        indexer++;
    }
    int armiesToDeploy = this->pi->reinforcementPool;
    int armiesPerTerritory = floor(armiesToDeploy / allWeakest.size());
    armiesToDeploy -= armiesPerTerritory * allWeakest.size();
    for (int i = 0;i < allWeakest.size();i++) {
        this->pi->orders->add(new DeployOrder(this->pi, allWeakest[i], armiesPerTerritory));
    }
    if (armiesToDeploy > 0) {
        this->pi->orders->add(new DeployOrder(this->pi, allWeakest[0], armiesToDeploy));
    }
    this->pi->reinforcementPool = 0;
    for (int i = 0;i < this->pi->hand->cardsInHand.size();i++) {
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "REINFORCEMENT") {
            this->pi->orders->add(new DeployOrder(this->pi, allWeakest[i], 10));
        }
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "BLOCKADE") {
            this->pi->orders->add(new BlockadeOrder(this->pi, allWeakest[i]));
        }
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "AIRLIFT") {
            this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[i], allWeakest[i + 1], floor(allWeakest[i]->getArmyCount() / 2)));
        }
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "DIPLOMACY") {
            int targetID;
            int maxID = this->pi->currentGame->getNumPlayers() - 1;
            if (this->pi->playerID == maxID) {
                targetID = maxID - 1;
            } else if (this->pi->playerID == 0) {
                targetID = 1;
            } else {
                targetID = this->pi->playerID + 1;
            }
            this->pi->orders->add(new NegotiateOrder(this->pi, this->pi->currentGame->getPlayerByID(targetID)));
        }
    }
}

//Benevolent players do not attack others' territories, hence an empty list is returned.
std::vector<Territory *> BenevolentPlayerStrategy::toAttack() {
    std::vector<Territory *> emptyList;
    return emptyList;
}

std::vector<Territory *> BenevolentPlayerStrategy::toDefend() {
    //List of territories which can be defended.
    std::vector<Territory*> defendList;

    for (int i = 0;i < (*this->pi->territories).size();i++) {
        defendList.push_back((*this->pi->territories)[i]);
    }
    //Territories should be sorted by lowest number of army units so as to prioritize "weaker" territories.
    auto armyComparator = [](Territory* t1, Territory* t2) { return t1->getArmyCount() <= t2->getArmyCount(); };
    std::sort(defendList.begin(), defendList.end(), armyComparator);
    return defendList;
}


