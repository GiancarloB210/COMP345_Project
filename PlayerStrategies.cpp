#include "PlayerStrategies.h"
#include <cmath>

PlayerStrategy::PlayerStrategy(Player* pi) {
    this->pi = pi;
}

#pragma region HumanPlayer
HumanPlayerStrategy::HumanPlayerStrategy(Player* pi) : PlayerStrategy(pi) {
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
    //Ensures that deployment orders are issued to one's owned territories until all of a player's army units are used up.
    while (numUnitsRemaining > 0) {
        cout << "Army units left for distribution: " << numUnitsRemaining << endl;
        defendIndex = 0;
        validTerritorySelectedCheck = false;
        // Prevents the user from inputting a non-numerical input or out-of-bounds index.
        while (!validTerritorySelectedCheck) {
            cout << "Enter the index of the territory which you would like to defend." << endl;
            cin >> defendIndex;
            if (cin.fail() || (defendIndex < 0 || defendIndex >(*this->pi->territories).size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid index." << endl;
            }
            else {
                validTerritorySelectedCheck = true;
            }
        }

        armiesSent = 0;
        validArmyNumberCheck = false;
        // Prevents the user from inputting a non-numerical input or out-of-bounds index.
        while (!validArmyNumberCheck) {
            cout << "Enter the number of army units you would like to deploy to " << defendList[defendIndex]->getName() << endl;
            cin >> armiesSent;
            if (cin.fail() || (armiesSent < 0 || armiesSent > numUnitsRemaining)) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid number of army units." << endl;
            }
            else {
                validArmyNumberCheck = true;
                numUnitsRemaining -= armiesSent;
                this->pi->orders->add(new DeployOrder(this->pi, defendList[defendIndex], armiesSent));
            }
        }

        cout << "List of territories to be defended (in order of highest priority): " << endl;
        for (int i = 0; i < this->pi->orders->getList().size(); i++) {
            cout << "[" << i << "] " << (dynamic_cast<DeployOrder*>(this->pi->orders->getList()[i]))->getTarget()->getName() << " with " << (dynamic_cast<DeployOrder*>(this->pi->orders->getList()[i]))->getArmiesToAdd() << " army units to be sent." << endl;
        }
    }
    //Update the player's reinforcement pool (now exhausted) accordingly.
    this->pi->reinforcementPool = 0;

    //Get information of every one of a player's newly owned (even after deployment orders) territories and the territories in them,
    std::vector<TempTerritoryUnitInfo> infoList = this->pi->getTempTerritoryUnitInfo();

    int continueAttackOrders = true;
    string continueResponse;
    bool continueResponseCheck = false;
    string attackOrDefend;
    bool attackOrDefendResponseCheck = false;
    while (continueAttackOrders) {
        //Determine if more deployment (offensive or defensive) orders are to be established.
        //Ensures that no invalid input can be entered.
        while (!continueResponseCheck) {
            attackOrDefend = "";
            attackOrDefendResponseCheck = false;
            cout << "Would you like to deploy some of your units to other countries? [Yes/No]" << endl;
            cin >> continueResponse;
            if (cin.fail() || (continueResponse != "Yes" && continueResponse != "No")) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid response." << endl;
            }
            else {
                continueResponseCheck = true;
                if (continueResponse == "No") {
                    continueAttackOrders = false;
                }
            }
        }
        //Break out of the while loop right away if no more orders are to be established.
        if (continueAttackOrders == false) {
            break;
        }
        //Determine if either an offensive or a defensive order is to be established.
        //Ensures that no invalid input can be entered.
        while (!attackOrDefendResponseCheck) {
            cout << "Would you like to attack an opposing territory or defend one of yours? [Attack/Defend]" << endl;
            cin >> attackOrDefend;
            if (cin.fail() || (attackOrDefend != "Attack" && attackOrDefend != "Defend")) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid response." << endl;
            }
            else {
                attackOrDefendResponseCheck = true;
            }
        }

        if (attackOrDefend == "Defend") {
            //Index of the source territory.
            int fromIndex_Defend = 0;
            //Index of the target territory.
            int toIndex_Defend = 0;
            //Source and target territory.
            Territory* fromTerritory = NULL;
            Territory* toTerritory = NULL;
            //Number of army units in source territory.
            int numUnitsOfFromTerritory = 0;
            //Number of units to be sent to target territory.
            int numUnitsSent = 0;
            //Command validity checks.
            bool validDefendCommandCheck = false;
            bool validFromIndexCheck = false;
            bool validToIndexCheck = false;
            bool validUnitsSentCheck = false;
            while (!validDefendCommandCheck) {
                cout << "List of owned territories of player " << this->pi->playerID << " with units in them:" << endl;
                int indexer = 0;
                //By "with units", this include units already there and units to be sent there after deployment order execution.
                std::vector<Territory*> territoriesWithUnits;
                for (int i = 0; i < infoList.size(); i++) {
                    //The info list is accessed to check for units to be sent via deployment orders.
                    if (infoList[i].numUnitsThere > 0) {
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout << "[" << indexer << "] " << infoList[i].territory->getName() << " (with " << infoList[i].numUnitsThere << " units)" << endl;
                        indexer++;
                    }
                }
                //Allows for a source territory to be selected.
                //Ensures that no invalid input can be entered.
                while (!validFromIndexCheck) {
                    cout << "Enter the index of the territory which you would like to deploy units from" << endl;
                    cin >> fromIndex_Defend;
                    if (cin.fail() || (fromIndex_Defend < 0 || fromIndex_Defend > territoriesWithUnits.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    }
                    else {
                        validFromIndexCheck = true;
                        fromTerritory = territoriesWithUnits[fromIndex_Defend];
                    }
                }


                indexer = 0;
                //All adjacent territories to the source territory that can have territories sent to them (are owned by the player).
                std::vector<Territory*> adjTerritoriesToFrom;
                for (int i = 0; i < defendList.size(); i++) {
                    std::vector<Territory*> adjacentToCurrent = defendList[i]->getAdjacentTerritories();
                    //If any territory in the defend list is found in the adjacent territories list
                    if (std::find(adjacentToCurrent.begin(), adjacentToCurrent.end(), fromTerritory) != adjacentToCurrent.end()) {
                        //If the player owns both the territory where units are being sent from and the territory to defend, but the territories are not the same..
                        if (fromTerritory->getPlayer() == defendList[i]->getPlayer() && fromTerritory->getName() != defendList[i]->getName()) {
                            //If the territory is not already in the list.
                            if (std::find(adjTerritoriesToFrom.begin(), adjTerritoriesToFrom.end(), defendList[i]) == adjTerritoriesToFrom.end()) {
                                adjTerritoriesToFrom.push_back(defendList[i]);
                                cout << "[" << indexer << "] " << defendList[i]->getName() << endl;
                                indexer++;
                            }
                        }
                    }
                }
                //Allows for a target territory to be selected.
                //Ensures that no invalid input can be entered.
                while (!validToIndexCheck) {
                    cout << "Enter the index of the territory which you would like to deploy units to" << endl;
                    cin >> toIndex_Defend;
                    if (cin.fail() || (toIndex_Defend < 0 || toIndex_Defend > adjTerritoriesToFrom.size())) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    }
                    else {
                        validToIndexCheck = true;
                        toTerritory = adjTerritoriesToFrom[toIndex_Defend];
                    }
                }

                //Checks the info list for the number of units of the source territory.
                for (int i = 0; i < infoList.size(); i++) {
                    if (infoList[i].territory->getName() == fromTerritory->getName()) {
                        numUnitsOfFromTerritory = infoList[i].numUnitsThere;
                    }
                }

                //Allows for the number of sent army units to be entered.
                //Ensures that no invalid input can be entered.
                while (!validUnitsSentCheck) {
                    cout << "Enter the number of units you would like to send." << endl;
                    cin >> numUnitsSent;
                    if (cin.fail() || (numUnitsSent < 0 || numUnitsSent > numUnitsOfFromTerritory)) {
                        cin.clear();
                        cin.ignore();
                        cout << "Invalid. Please enter a valid index." << endl;
                    }
                    else {
                        validUnitsSentCheck = true;
                        //Takes army units from the source territory and sends them to the target territory.
                        //Adds the corresponding advancement order afterwards.
                        for (int j = 0; j < infoList.size(); j++) {
                            if (infoList[j].territory->getName() == fromTerritory->getName()) {
                                infoList[j].numUnitsThere -= numUnitsSent;
                            }
                            if (infoList[j].territory->getName() == toTerritory->getName()) {
                                infoList[j].numUnitsThere += numUnitsSent;
                            }
                        }
                        cout << "Deploying " << numUnitsSent << " from " << fromTerritory->getName() << " to " << toTerritory->getName() << endl;
                        this->pi->orders->add(new AdvanceOrder(this->pi, fromTerritory, toTerritory, numUnitsSent));
                    }
                }
                validDefendCommandCheck = true;
                //Ensures that another continuation choice can be made afterwards.
                continueResponseCheck = false;
                cout << "List of owned territories of player " << this->pi->playerID << " with units in them:" << endl;
                indexer = 0;
                for (int i = 0; i < infoList.size(); i++) {
                    if (infoList[i].numUnitsThere > 0) {
                        //Note: The line directly underneath this comment may serve no ultimate purpose, but is kept here just in case the program crashes without it.
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout << "[" << indexer << "] " << infoList[i].territory->getName() << " (with " << infoList[i].numUnitsThere << " units)" << endl;
                        indexer++;
                    }
                }
            }
        }

        if (attackOrDefend == "Attack") {
            //See the ("attackOrDefend == "Attack") block for informstion about what each of these
            //extremely similar fields pertain to.
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
                cout << "List of owned territories of player " << this->pi->playerID << " with units in them:" << endl;
                int indexer = 0;
                //All of the current player's owned territories that either have or will have (after deployment order execution)
                //territories in them.
                std::vector<Territory*> territoriesWithUnits;
                for (int i = 0; i < infoList.size(); i++) {
                    if (infoList[i].numUnitsThere > 0) {
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout << "[" << indexer << "] " << infoList[i].territory->getName() << " (with " << infoList[i].numUnitsThere << " units)" << endl;
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
                    }
                    else {
                        validFromIndexCheck = true;
                        fromTerritory = territoriesWithUnits[fromIndex_Attack];
                    }
                }
                indexer = 0;
                std::vector<Territory*> adjacentTerritoriesToAttack;
                for (int i = 0; i < attackList.size(); i++) {
                    std::vector<Territory*> adjacentToCurrent = attackList[i]->getAdjacentTerritories();
                    //If the source territory is found in the adjacent territoru list of the candidate target territory (is adjacent to it)
                    if (std::find(adjacentToCurrent.begin(), adjacentToCurrent.end(), fromTerritory) != adjacentToCurrent.end()) {
                        //If the same player does not own both territories (criterion for attack target)
                        if (fromTerritory->getPlayer() != attackList[i]->getPlayer()) {
                            //If the candidate attack target is not already in the list
                            if (std::find(adjacentTerritoriesToAttack.begin(), adjacentTerritoriesToAttack.end(), attackList[i]) == adjacentTerritoriesToAttack.end()) {
                                adjacentTerritoriesToAttack.push_back(attackList[i]);
                                cout << "[" << indexer << "] " << attackList[i]->getName() << endl;
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
                    }
                    else {
                        validToIndexCheck = true;
                        toTerritory = adjacentTerritoriesToAttack[toIndex_Attack];
                    }
                }
                for (int i = 0; i < infoList.size(); i++) {
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
                    }
                    else {
                        validUnitsSentCheck = true;
                        for (int j = 0; j < infoList.size(); j++) {
                            if (infoList[j].territory->getName() == fromTerritory->getName()) {
                                infoList[j].numUnitsThere -= numUnitsSent;
                            }
                            if (infoList[j].territory->getName() == toTerritory->getName()) {
                                infoList[j].numUnitsThere += numUnitsSent;
                            }
                        }
                        cout << "Deploying " << numUnitsSent << " from " << fromTerritory->getName() << " to attack " << toTerritory->getName() << endl;
                        this->pi->orders->add(new AdvanceOrder(this->pi, fromTerritory, toTerritory, numUnitsSent));
                    }
                }
                validAttackCommandCheck = true;
                continueResponseCheck = false;
                cout << "List of owned territories of player " << this->pi->playerID << " with units in them:" << endl;
                indexer = 0;
                for (int i = 0; i < infoList.size(); i++) {
                    if (infoList[i].numUnitsThere > 0) {
                        territoriesWithUnits.push_back(infoList[i].territory);
                        cout << "[" << indexer << "] " << infoList[i].territory->getName() << " (with " << infoList[i].numUnitsThere << " units)" << endl;
                        indexer++;
                    }
                }
            }
        }
    }
    //Whether a card will be played or not.
    bool willPlayCard = false;
    //Yes/No response for card playing.
    string willPlayCardStr;
    //Whether the overall response for card playing is valid
    bool validCardResponse = false;
    //Index of the played card in the current player's hand vector
    int cardIndex = 0;
    //Whether a valid index has been chosen.
    bool validCardIndexChosen = false;
    //The selected card (never used as of yet, but could be in the future).
    Card* chosenCard;
    while (!validCardResponse) {
        cout << "Would you like to play a card from your hand? [Yes/No]" << endl;
        cin >> willPlayCardStr;
        if (cin.fail() || (willPlayCardStr != "Yes" && willPlayCardStr != "No")) {
            cin.clear();
            cin.ignore();
            cout << "Invalid. Please enter a valid response." << endl;
        }
        else {
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
        //Resets this value in case the method is called again at a later point.
        willPlayCardStr = "";
        //Lists all of the cards in terms of their type (diplomacy, reinforcement, etc.) in the current player's hand.
        cout << "Cards in the hand of player " << this->pi->playerID << endl;
        int indexer = 0;
        for (int i = 0; i < this->pi->hand->cardsInHand.size(); i++) {
            cout << "[" << indexer << "] " << this->pi->hand->cardsInHand[i]->getCardTypeStringValue() << endl;
            indexer++;
        }
        while (!validCardIndexChosen) {
            //Ensure that the index is valid and within player hand vector bounds.
            cout << "Enter the index of the card which you would like to play." << endl;
            cin >> cardIndex;
            if (cin.fail() || (cardIndex < 0 || cardIndex > this->pi->hand->cardsInHand.size())) {
                cin.clear();
                cin.ignore();
                cout << "Invalid. Please enter a valid index." << endl;
            }
            else {
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

    //All territories should be able to be defended by a human player.
    cout << "List of territories owned by player " << this->pi->playerID << " that can be defended:" << endl;
    for (int i = 0; i < (*this->pi->territories).size(); i++) {
        defendList.push_back((*this->pi->territories)[i]);
        cout << "[" << i << "] " << (*this->pi->territories)[i]->getName() << endl;
    }
    return defendList;
}

std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    for (int i = 0; i < (*this->pi->territories).size(); i++) {
        for (int j = 0; j < (*this->pi->territories)[i]->getAdjacentTerritories().size(); j++) {
            //Only return an adjacent territory if it doesn't belong to the current player.
            if ((*this->pi->territories)[i]->getAdjacentTerritories()[j]->getPlayer() != this->pi) {
                attackList.push_back((*this->pi->territories)[i]->getAdjacentTerritories()[j]);
            }
        }
    }
    return attackList;
}

#pragma endregion

#pragma region AggressivePlayer
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* pi) : PlayerStrategy(pi) {
    this->pi = pi;
}

void AggressivePlayerStrategy::issueOrder() {
    cout << "Aggressive Player order issue" << endl;
    //Territories owned by the player, ordered by number of units.
    std::vector<Territory*> orderedByStrongest = this->toDefend();
    cout << "Finished toDefend() [Aggressive]" << endl;
    //The first territory in the returned list is the one with the most territories.
    if (orderedByStrongest.size() == 0) {
        cout<<"Player "<<this->pi->playerID<<" has no more territories left to defend."<<endl;
        return;
    }
    Territory* strongest = orderedByStrongest[0];
    std::vector<Territory*> adjacentToAttack;
    std::vector<Territory*> adjacent = strongest->getAdjacentTerritories();
    cout << "Finished getAdjacentTerritories() [Aggressive]" << endl;
    cout << "About to enter adjacency for loop" << endl;
    //If an adjacent territory to the strongest territory is not owned by the player, add it to the list of possible territories to attack.
    for (int j = 0; j < adjacent.size(); j++) {
        if (adjacent[j]->getPlayer() != this->pi) {
            adjacentToAttack.push_back(adjacent[j]);
        }
    }
    cout << "adjacentToAttack initialized" << endl;
    //Send all army units of the aggressive player to its strongest territory.
    int armiesToDeploy = this->pi->reinforcementPool;
    this->pi->orders->add(new DeployOrder(this->pi, strongest, armiesToDeploy));
    cout << "DeployOrder added" << endl;
    //Update the reinforcement pool to match the depletion of army units.
    this->pi->reinforcementPool = 0;
    //If there are no adjacent enemy territories to attack, do nothing else.
    if (adjacentToAttack.empty()) {
        cout << "No adjacent territories to attack!" << endl;
        return;
    }
    else {
        cout << "List of adjacent territories to attack:" << endl;
        for (int i = 0; i < adjacentToAttack.size(); i++) {
            cout << "[" << i << "] " << adjacentToAttack[i]->getName() << endl;
        }
    }
    //Determine how many units (ideally an equal amount) to send to each adjacent enemy territory for attacking.
    int armiesPerEnemyTerritory = floor(armiesToDeploy / adjacentToAttack.size());
    armiesToDeploy -= armiesPerEnemyTerritory * adjacentToAttack.size();
    //Issue attack orders for each of the adjacent territories able to be attacked.
    int numAdvanceAdded = 0;
    for (int i = 0; i < adjacentToAttack.size(); i++) {
        this->pi->orders->add(new AdvanceOrder(this->pi, strongest, adjacentToAttack[i], armiesPerEnemyTerritory));
        numAdvanceAdded++;
    }
    cout << numAdvanceAdded << " AdvanceOrders added" << endl;
    numAdvanceAdded = 0;
    //Send any remaining owned army units to attack some territory in the filtered attack list.
    if (armiesToDeploy > 0 && adjacentToAttack.empty() == false) {
        this->pi->orders->add(new AdvanceOrder(this->pi, strongest, adjacentToAttack[0], armiesToDeploy));
        numAdvanceAdded++;
    }
    cout << numAdvanceAdded << " more AdvanceOrders added" << endl;
    //If the player has a bomb card in their hand and there are actually territories adjacent to the
    //strongest to attack, issue a bomb order to some adjacent territory.
    bool bombAdded = false;
    int adjacencyTracker = 0;
    int numBombOrders = 0;
    for (int i = 0; i < this->pi->hand->cardsInHand.size(); i++) {
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "BOMB" && adjacentToAttack.empty() == false) {
            //The bomb order must go before all other orders in case any advance orders result in others' territories becoming your own post-battle.
            std::vector<Order*> list = this->pi->orders->getList();
            list.insert(list.begin(), new BombOrder(this->pi, adjacentToAttack[adjacencyTracker]));
            this->pi->orders->setList(list);
            numBombOrders++;
            //Only increment the adjacency tracker if there are more adjacent territories to attack.
            if (adjacencyTracker < adjacentToAttack.size() - 1) {
                adjacencyTracker++;
            }
            bombAdded = true;
        }
    }
    cout << (bombAdded == true ? numBombOrders + " BombOrders added" : "BombOrder not added") << endl;
}

std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
    //List of territories which can be defended.
    std::vector<Territory*> defendList;
    defendList.clear();
    for (int i = 0; i < (*this->pi->territories).size(); i++) {
        defendList.push_back((*this->pi->territories)[i]);
        Territory* territory = defendList[i];
        cout << i << ": " << territory->getName() << " with units = " << territory->getArmyCount() << " and belonging to player " << territory->getPlayer()->getName() << endl;
    }
    cout << "Finished toDefend() for loop [Aggressive], and defendList has size " << defendList.size() << endl;
    //Sorts by highest number of army units.
    auto armyComparator = [](Territory* t1, Territory* t2) { return (*t1).getArmyCount() > (*t2).getArmyCount(); };
    cout << "Set up comparator [Aggressive]" << endl;
    std::sort(defendList.begin(), defendList.end(), armyComparator);
    cout << "Finished sorting defendList [Aggressive]" << endl;
    return defendList;
}

std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    for (int i = 0; i < (*this->pi->territories).size(); i++) {
        //Only attack an adjacent territory if the current territory actually has armies in it.
        if ((*this->pi->territories)[i]->getArmyCount() > 0) {
            for (int j = 0; j < (*this->pi->territories)[i]->getAdjacentTerritories().size(); j++) {
                //Only return an adjacent territory if it doesn't belong to the current player.
                if ((*this->pi->territories)[i]->getAdjacentTerritories()[j]->getPlayer() != this->pi) {
                    attackList.push_back((*this->pi->territories)[i]->getAdjacentTerritories()[j]);
                }
            }
        }
    }
    return attackList;
}

#pragma endregion

#pragma region BenevolentPlayer
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* pi) : PlayerStrategy(pi) {
    this->pi = pi;
}

void BenevolentPlayerStrategy::issueOrder() {
    cout << "in issueOrder (Benevolent)" << endl;
    //The returned list is sorted by lowest army unit count.
    std::vector<Territory*> orderedByWeakest = this->toDefend();
    //Don't do anything more if the returned list is empty.
    if (orderedByWeakest.empty()) {
        cout << "Player " << this->pi->playerID << " has no more territories left to defend!" << endl;
        return;
    }
    cout << "after toDefend(), size " << orderedByWeakest.size() << endl;
    //Will only contain those owned territories that have the strictly lowest number of army units.
    std::vector<Territory*> allWeakest;
    allWeakest.push_back(orderedByWeakest[0]);
    int minNumArmies = orderedByWeakest[0]->getArmyCount();
    //bool check for the while loop below
    bool minNumArmiesChecker = true;
    cout<<"About to enter orderedByWeakest if"<<endl;
    //This if check is necessary so that orderedByWeakest[indexer] does not go out of bounds if the array is of size 1.
    if (orderedByWeakest.size() > 1) {
        int indexer = 1;
        while (minNumArmiesChecker && orderedByWeakest[indexer]->getArmyCount() == minNumArmies) {
            cout<<"indexer: "<<indexer<<endl;
            allWeakest.push_back(orderedByWeakest[indexer]);
            cout<<"Pushed back"<<endl;
            indexer++;
            cout<<"indexer: "<<indexer<<", size: "<<orderedByWeakest.size()<<endl;
            if (indexer >= orderedByWeakest.size() - 1) {
                //Break condition for the while loop.
                cout<<"Breaking free"<<endl;
                minNumArmiesChecker = false;
            }
        }
    }
    cout << "initialized allWeakest" << endl;
    int armiesToDeploy = this->pi->reinforcementPool;
    //Allows for even allocation of armies in source territory to adjacent owned territories.
    int armiesPerTerritory = floor(armiesToDeploy / allWeakest.size());
    armiesToDeploy -= armiesPerTerritory * allWeakest.size();
    //Creates deploy orders for every territory with an evenly distributed number of territories.
    //If any excess units are left, allocate them to the first territory in the list so that all are used up.
    for (int i = 0; i < allWeakest.size(); i++) {
        this->pi->orders->add(new DeployOrder(this->pi, allWeakest[i], armiesPerTerritory));
    }
    if (armiesToDeploy > 0) {
        this->pi->orders->add(new DeployOrder(this->pi, allWeakest[0], armiesToDeploy));
    }
    //Update the numerical value of the player's army unit pool accordingly.
    this->pi->reinforcementPool = 0;
    cout << "finished first batch of DeployOrders" << endl;
    cout << "allWeakest size: " << allWeakest.size() << endl;
    cout << "orderedByWeakness size: " << orderedByWeakest.size() << endl;
    //Play all of the cards in the user's hand that don't serve any offensive purpose.
    for (int i = 0; i < this->pi->hand->cardsInHand.size(); i++) {
        cout << "Card at position " << i << ": " << this->pi->hand->cardsInHand[i]->getCardTypeStringValue() << endl;
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "REINFORCEMENT") {
            //Add 10 army units ("reinforcements") to some territory in the list of weakest owned territories.
            if (i > allWeakest.size() - 1) {
                this->pi->orders->add(new DeployOrder(this->pi, allWeakest[allWeakest.size() - 1], 10));
            }
            else {
                this->pi->orders->add(new DeployOrder(this->pi, allWeakest[i], 10));
            }
        }
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "BLOCKADE") {
            //Establishes a blockade for some territory in the list of weakest owned territories.
            if (i > allWeakest.size() - 1) {
                this->pi->orders->add(new BlockadeOrder(this->pi, allWeakest[allWeakest.size() - 1]));
            }
            else {
                this->pi->orders->add(new BlockadeOrder(this->pi, allWeakest[i]));
            }

        }
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "AIRLIFT") {
            //Transfers half the army units from some owned source territory to some owned target territory.
            //If the number of territories with the lowest army size is greater than 1, allocate some army units from one of them to another one of them.
            if (allWeakest.size() > 1) {
                if (i >= allWeakest.size() - 1) {
                    this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[allWeakest.size() - 1], allWeakest[allWeakest.size() - 2], floor(allWeakest[allWeakest.size() - 1]->getArmyCount() / 2)));
                }
                else {
                    this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[i], allWeakest[i + 1], floor(allWeakest[i]->getArmyCount() / 2)));
                }
            }
            //If there only exists 1 territory with the lowest number of army units, send half of them to
            //a territory with the second-lowest number of army units.
            else {
                bool backupCheck = true;
                int indexer = 0;
                while (backupCheck) {
                    //If the territory currently being assessed contains the SECOND-highest number of army units and is not the same as the source territory...
                    if (orderedByWeakest[indexer]->getArmyCount() > minNumArmies && allWeakest[i] != orderedByWeakest[indexer]) {
                        cout << "Match, indexer " << indexer << " and i " << i << endl;
                        //...send it half of the army units from the source territory.
                        if (i > allWeakest.size() - 1) {
                            this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[allWeakest.size() - 1], orderedByWeakest[indexer], floor(allWeakest[allWeakest.size() - 1]->getArmyCount() / 2)));
                        }
                        else {
                            cout << (allWeakest[i] == orderedByWeakest[indexer] ? "They're the same" : "They're not the same") << endl;
                            this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[i], orderedByWeakest[indexer], floor(allWeakest[i]->getArmyCount() / 2)));
                        }
                        backupCheck = false;
                    }
                    indexer++;
                    //If you reach the end of the list of territories without finding one to send army units to,
                    //just send army units to the last territory in the list and call it a day.
                    if (indexer == orderedByWeakest.size() - 1 && backupCheck == true) {
                        if (i > allWeakest.size() - 1) {
                            this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[allWeakest.size() - 1], orderedByWeakest[indexer], floor(allWeakest[allWeakest.size() - 1]->getArmyCount() / 2)));
                        }
                        else {
                            this->pi->orders->add(new AirliftOrder(this->pi, allWeakest[i], orderedByWeakest[indexer], floor(allWeakest[i]->getArmyCount() / 2)));
                        }
                        backupCheck = false;
                    }
                }
            }
        }
        if (this->pi->hand->cardsInHand[i]->getCardTypeStringValue() == "DIPLOMACY") {
            //Establish a diplomacy with some other player. The other player's ID is chosen such that
            //it does not go out of bounds of the array of game players.
            int targetID;
            int maxID = this->pi->currentGame->getNumPlayers() - 1;
            cout << "maxID: " << maxID << endl;
            if (this->pi->playerID == maxID) {
                cout << "Case 1: = maxID" << endl;
                targetID = maxID - 1;
            }
            else if (this->pi->playerID == 0) {
                cout << "Case 2: = 0" << endl;
                targetID = 1;
            }
            else {
                cout << "Case 3" << endl;
                targetID = this->pi->playerID + 1;
            }
            this->pi->orders->add(new NegotiateOrder(this->pi, this->pi->currentGame->getPlayerByID(targetID)));
        }
    }
}

//Benevolent players do not attack others' territories, hence an empty list is returned.
std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    return std::vector<Territory*>();
}

std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    //List of territories which can be defended.
    std::vector<Territory*> defendList;

    //Initially adds all owned territories to the list.
    for (int i = 0; i < (*this->pi->territories).size(); i++) {
        defendList.push_back((*this->pi->territories)[i]);
    }
    //Territories should be sorted by lowest number of army units so as to prioritize "weaker" territories.
    auto armyComparator = [](Territory* t1, Territory* t2) { return t1->getArmyCount() < t2->getArmyCount(); };
    std::sort(defendList.begin(), defendList.end(), armyComparator);
    return defendList;
}

#pragma endregion

#pragma region NeutralPlayer
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* pi) : PlayerStrategy(pi) {
    this->pi = pi;
}

void NeutralPlayerStrategy::issueOrder() {
    cout << "Neutral player does NOT issue any orders not plays any cards" << endl;
}

std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    std::vector<Territory*> defendList;

    return defendList;
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;

    return attackList;
}
#pragma endregion

#pragma region CheaterPlayer
CheaterPlayerStrategy::CheaterPlayerStrategy(Player* pi) : PlayerStrategy(pi) {
    this->pi = pi;
}

void CheaterPlayerStrategy::issueOrder() {
    cout << "Cheater player issues orders to capture adjacent territories that are not theirs" << endl;
    std::vector<Territory*> listToCapture = toAttack();
    int numCheaterOrdersAdded = 0;
    for (int i = 0; i < listToCapture.size(); i++) {
        this->pi->orders->add(new CheaterOrder(this->pi, listToCapture[i]));
        numCheaterOrdersAdded++;
    }
    cout << numCheaterOrdersAdded << " CheaterOrders added" << endl;
}

std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
    std::vector<Territory*> defendList;

    return defendList;
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    Territory* temp;
    bool uniqueTerritory = true;
    for (int i = 0; i < (*this->pi->territories).size(); i++) {
        //Targets an adjacent territory regardless if the current territory has armies
        for (int j = 0; j < (*this->pi->territories)[i]->getAdjacentTerritories().size(); j++) {
            //Only return an adjacent territory if it doesn't belong to the current player.
            if ((*this->pi->territories)[i]->getAdjacentTerritories()[j]->getPlayer() != this->pi) {
                //Makes sure target territory is NOT already in the list
                for (int k = 0; k < attackList.size(); k++)
                {
                    if (attackList[k] == (*this->pi->territories)[i]->getAdjacentTerritories()[j])
                        uniqueTerritory = false;
                }
                if (uniqueTerritory)
                    attackList.push_back((*this->pi->territories)[i]->getAdjacentTerritories()[j]);
            }
            uniqueTerritory = true;
        }
        
    }
    return attackList;
}
#pragma endregion