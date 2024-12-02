#include "Orders.h"
#include "Map.h"


using namespace std;
bool GetCard = false;
std::list<Player*> Armistice;
#pragma region OrderListClass
//OrderList Class
//--------------------------------------------------------------------------------------------------------

//Constructors
OrderList::OrderList() //Default Constructor
{
	this->ListofOrders = std::vector<Order*>();
}
OrderList::OrderList(std::vector<Order*> list) //Parameter Constructor
{
	ListofOrders = list;
}
OrderList::OrderList(OrderList& orderlist) //Copy Constructor
{
	ListofOrders = orderlist.ListofOrders;
}

//Deconstructor
OrderList::~OrderList() {}

//Accessors
std::vector<Order*> OrderList::getList()
{
	return ListofOrders;
}

void OrderList::setList(std::vector<Order*> list) {
	this->ListofOrders = list;
}

//Returns the global variable accessors
std::list<Player*> OrderList::getArmistice()
{
	return Armistice;
}

bool OrderList::isGettingCard()
{
	return GetCard;
}

//Methods
void OrderList::add(Order* order) //Adds Order to beginning of list
{
	ListofOrders.push_back(order);
}

void OrderList::move(int iPos, int fPos) //Moves Order in position X to go above position Y
{
	std::vector<Order*> newList;
	if ((iPos > 0 && iPos <= ListofOrders.size()) && (fPos > 0 && fPos <= ListofOrders.size())) //Verifies that parameters are valid
	{
		bool block = true; //Used to make a one time use if statement
		Order* OrdertoMove = ListofOrders.at(iPos - 1);
		for (int i = 0; i < ListofOrders.size(); i++)
		{
			if (iPos == fPos)
			{
				newList.push_back(ListofOrders.at(i));
			}
			else if ((i == iPos - 1))
			{
				//Does nothing, just skips index
			}
			else if ((i == fPos - 1) && block)
			{
				newList.push_back(OrdertoMove);
				i--; //There is an order already at this index, so we reduce i to repeat loop and not skip it
				block = false;
			}
			else
			{
				newList.push_back(ListofOrders.at(i));
			}
		}
		ListofOrders = newList;
	}
	else
	{
		cout << "Error: " << iPos << " or " << fPos << " are invalid positions";
	}
}

void OrderList::remove(int Pos)
{
	std::vector<Order*> newList;

	if (Pos > 0 && Pos <= ListofOrders.size()) //Verifies that parameters are valid
	{
		for (int i = 0; i < ListofOrders.size(); i++)
		{
			if (i == Pos - 1)
			{
				//Does nothing, just skips index
			}
			else
			{
				newList.push_back(ListofOrders.at(i));
			}
		}
		ListofOrders = newList;
	}
	else
	{
		cout << "Error: " << Pos << " is an invalid position";
	}
}




//Operators
std::ostream& operator << (std::ostream& os, OrderList& orderlist)
{
	std::string ListNames;
	for (int i = 0; i < orderlist.getList().size(); i++)
	{
		ListNames.append("\n\t" + (i + 1) + orderlist.getList().at(i)->getType());
	}
	return (os << "List of Orders:\n" << ListNames);
}
OrderList& OrderList::operator=(const OrderList& order) = default;
#pragma endregion

#pragma region OrderClass
//Order Class
//--------------------------------------------------------------------------------------------------------

//Constructors
Order::Order() { isDefaultOrder = true; } //Default Constructor (Used for subclasses)
Order::Order(Player* CurrentPlayer, std::string type)  //Parameter Constructor (Used to create other orders)
{
	this->CurrentPlayer = CurrentPlayer;
	this->type = type;
	description = "This order has the name " + type + " and it is unsupported";
	effect = "This order does not to anything";
	valid = false; //Every order that is not one of the subclasses will be invalid by default
	isDefaultOrder = false;
}
Order::Order(Order& order) //Copy Constructor
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
Order::~Order() {}

//Accessors
std::string Order::getType() {
	return type;
}
std::string Order::getDescription() {
	return description;
}
std::string Order::getEffect() {
	return effect;
}

//Methods
bool Order::validate()
{
	return valid;
}
void Order::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid";
	else if (validate())
	{
		cout << "Nope";
	}
	else
	{
		cout << "Order no valid";
	}
}

//Operators
std::ostream& operator << (std::ostream& os, Order& order)
{
	return (os << "Order: " << order.getType() << "\nDescription: " << order.getDescription() << "\nEffect: " << order.getEffect());
}
Order& Order::operator=(const Order& order) = default;

#pragma endregion


#pragma region DeployOrder
//DeployOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
DeployOrder::DeployOrder()
{
	type = "Deploy";
	description = "Add a set number of troops to a location in the map";
	effect = "Adds a number to the value of present troops in a territory";
	valid = false;
	isDefaultOrder = true;
}

DeployOrder::DeployOrder(Player* CurrentPlayer, Territory* target, int ATD)
{
	this->CurrentPlayer = CurrentPlayer;
	this->target = target;
	ArmiesToAdd = ATD;
	type = "Deploy";
	description = "Add a set number of troops to a location in the map";
	effect = "Adds a number to the value of present troops in a territory";
	valid = false;
	isDefaultOrder = false;
}
DeployOrder::DeployOrder(DeployOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
DeployOrder::~DeployOrder() {}

Territory* DeployOrder::getTarget() {
	return this->target;
}

int DeployOrder::getArmiesToAdd() {
	return this->ArmiesToAdd;
}

//methods
bool DeployOrder::validate()
{
	cout << "Validating deployOrder" << endl;
	cout << "Target: " << target->getName() << endl;
	if (CurrentPlayer == target->getPlayer()) {
		//Verify that current player is the same as territory owner
		cout << "deployOrder validated!" << endl;
		valid = true;
	}
	return valid;
}
void DeployOrder::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid\n";
	else if (validate())
	{
		int TotalArmies = ArmiesToAdd + target->getArmyCount();
		target->setArmyCount(TotalArmies);
		cout << target->getName() << " has had armies deployed to it, and now contains " << target->getArmyCount() << " armies." << endl;

	}
	else
	{
		cout << "Deploy Order is not valid\n";
	}
}

//Operators
DeployOrder& DeployOrder::operator=(const DeployOrder& order) = default;

#pragma endregion

#pragma region AdvanceClass
//AdvanceOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
AdvanceOrder::AdvanceOrder()
{
	type = "Advance";
	description = "Move a group of units from a territory to the other. Battle will take place if the destination is an enemy territory or neutral territory";
	effect = "Remove a number of troops from the initial territory, then add the same number to the next territory. Battle functions will be launch the destination is not friendly";
	valid = false;
	isDefaultOrder = true;
}

AdvanceOrder::AdvanceOrder(Player* CurrentPlayer, Territory* source, Territory* target, int ATA)
{
	this->CurrentPlayer = CurrentPlayer;
	this->source = source;
	this->target = target;
	ArmiesToAdvance = ATA;
	type = "Advance";
	description = "Move a group of units from a territory to the other. Battle will take place if the destination is an enemy territory or neutral territory";
	effect = "Remove a number of troops from the initial territory, then add the same number to the next territory. Battle functions will be launch the destination is not friendly";
	valid = false;
	isDefaultOrder = false;
}
AdvanceOrder::AdvanceOrder(AdvanceOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
AdvanceOrder::~AdvanceOrder() {}

//methods
bool AdvanceOrder::validate()
{
	//Cannot attack or advance to a territory with no owner (one in a blockade).
	if (target->getPlayer() == nullptr) {
		return false;
	}
	if (CurrentPlayer == source->getPlayer()) //First, we check if player of source territory is the same as the current player
	{
		cout << "AdvanceOrder: source player verification complete" << endl;
		//Then, we check through the list of adjacent territories of the source and see if one is equal to the target
		for (std::string x : source->getAdjacentTerritoryNames())
		{
			if (target->getName() == x)
			{
				cout << "AdvanceOrder: target name located in adjacent territory list." << endl;
				valid = true;
			}
		}
	}

	return valid;
}
void AdvanceOrder::execute()
{
	if (isDefaultOrder)
	{
		cout << "This order is default and invalid\n";
		return;
	}
	int TotalArmies;
	int TargetArmies;
	bool battle = true;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(0, 10);
	int randomValue;

	if (validate())
	{
		if (CurrentPlayer == target->getPlayer()) //If target territory is player territory
		{
			cout << "AdvanceOrder: deploying to another territory owned by player " << CurrentPlayer->playerID << endl;
			//First, move armies by setting a new amount of armies in the target
			TotalArmies = ArmiesToAdvance + target->getArmyCount();
			target->setArmyCount(TotalArmies);
			cout << "Target " << target->getName() << " -> number of armies increased to " << TotalArmies << endl;
			//Then, substract current number of armies in source by removing number of armies that moved
			TotalArmies = source->getArmyCount() - ArmiesToAdvance;
			source->setArmyCount(TotalArmies);
			cout << "Source " << source->getName() << " -> number of armies decreased to " << TotalArmies << endl;
		}
		else
		{
			cout << "A battle is about to begin between players " << source->getPlayer()->getName() << " (attacking) and " << target->getPlayer()->getName() << " (defending)" << endl;
			//We check if we have an agreement with the current holder of the target territory
			for (Player* p : Armistice)
			{
				if (target->getPlayer() == p)
				{
					cout << "Order is invalid since we are attacking a player that has a not agression pact with us this turn\n";
					return;
				}
			}
			//First, we substract current number of armies in source by removing number of armies that moved
			TotalArmies = source->getArmyCount() - ArmiesToAdvance;
			source->setArmyCount(TotalArmies);
			cout << "Source " << source->getName() << " -> number of armies changed to " << TotalArmies << endl;

			//Then we set the number of enemies to fight
			TargetArmies = target->getArmyCount();
			while (battle)
			{
				if (TargetArmies == 0 || ArmiesToAdvance == 0)
					battle = false;
				else
				{
					randomValue = distrib(gen);
					if (randomValue <= 6)
						cout << target->getPlayer()->getName() << " (the defender) lost one army!" << endl;
					TargetArmies -= 1;

					randomValue = distrib(gen);
					if (randomValue <= 7)
						cout << source->getPlayer()->getName() << " (the attacker) lost one army!" << endl;
					ArmiesToAdvance -= 1;
				}
			}

			//After battle, we see if attacker won and has troups left. Else, nothing happens
			if (TargetArmies == 0 && ArmiesToAdvance != 0)
			{
				cout << CurrentPlayer->name << " has won the battle between their owned " << source->getName() << " and the opponent's " << target->getName() << endl;
				target->setArmyCount(ArmiesToAdvance);
				target->setPlayer(CurrentPlayer);
				//The player who won draws a card if possible and if they haven't already drawn any after playing orders this turn.
				if (CurrentPlayer->hand->deckPlayedWith->getNumDrawableCards() > 0 && CurrentPlayer->drewCard == false) {
					CurrentPlayer->hand->drawCard();
					cout << "Player " << CurrentPlayer->playerID << " drew a " << CurrentPlayer->hand->cardsInHand[CurrentPlayer->hand->cardsInHand.size() - 1]->getCardTypeStringValue() << " card." << endl;
					GetCard = true;
					CurrentPlayer->drewCard = true;
				} else {
					cout << "Player " << CurrentPlayer->playerID << " cannot draw any more cards because there are no more cards left to draw."<<endl;
					GetCard = false;
				}
				cout << "Target " << target->getName() << " -> number of armies changed to " << ArmiesToAdvance << endl;
			}
			else {
				cout << CurrentPlayer->name << " has lost the battle between their owned " << source->getName() << " and the opponent's " << target->getName() << endl;
			}

			target->getPlayer()->gotAttacked = true; //Value changed at the end of an attack order if it is executed successfully
		}
	}
	else
	{
		cout << "Order to Advance is not valid\n";
	}
}

//Operators
AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& order) = default;
#pragma endregion

#pragma region BombClass
//BombOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
BombOrder::BombOrder()
{
	type = "Bomb";
	description = "Does damage to the units in the target territory";
	effect = "Removes a number of hostile troops on a selected territory";
	valid = false;
	isDefaultOrder = true;
}

BombOrder::BombOrder(Player* CurrentPlayer, Territory* target)
{
	this->CurrentPlayer = CurrentPlayer;
	this->target = target;
	type = "Bomb";
	description = "Does damage to the units in the target territory";
	effect = "Removes a number of hostile troops on a selected territory";
	valid = false;
	isDefaultOrder = false;
}
BombOrder::BombOrder(BombOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
BombOrder::~BombOrder() {}

//methods
bool BombOrder::validate()
{
	if (CurrentPlayer != target->getPlayer()) //First, we check if player of source territory is NOT the same as the current player
	{
		//Then, we check through the list of adjacent territories of the target and see if one is the territory of the Current player
		for (Territory* x : target->getAdjacentTerritories())
		{
			if (x->getPlayer() == CurrentPlayer)
			{
				valid = true;
			}
		}
	}

	return valid;
}
void BombOrder::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid\n";
	else if (validate())
	{
		//We check if we have an agreement with the current holder of the target territory
		for (Player* p : Armistice)
		{
			if (target->getPlayer() == p)
			{
				cout << "Order is invalid since we are attacking a player that has a not agression pact with us this turn\n";
				return;
			}
		}

		//Destroy half of the armies in the target territory, and set that half-value as their new army count value.
		int TotalArmy = target->getArmyCount();
		TotalArmy = TotalArmy / 2;
		target->setArmyCount(TotalArmy);
		cout << target->getName() << " has been bombed and now has " << target->getArmyCount() << " armies left." << endl;

		target->getPlayer()->gotAttacked = true; //Value changed at the end of an attack order if it is executed successfully
	}
	else
	{
		cout << "Order to bomb is not valid\n";
	}
}

//Operators
BombOrder& BombOrder::operator=(const BombOrder& order) = default;
#pragma endregion

#pragma region BlockadeOrder
//BlockadeOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
BlockadeOrder::BlockadeOrder()
{
	type = "Blockade";
	description = "Increase the amount of troops in a friendly territory and transforms that territory into neutral territory";
	effect = "Triples the value of troops in a owned territory and changes designation to neutral";
	valid = false;
	isDefaultOrder = true;
}

BlockadeOrder::BlockadeOrder(Player* CurrentPlayer, Territory* target)
{
	this->CurrentPlayer = CurrentPlayer;
	this->target = target;
	type = "Blockade";
	description = "Increase the amount of troops in a friendly territory and transforms that territory into neutral territory";
	effect = "Triples the value of troops in a owned territory and changes designation to neutral";
	valid = false;
	isDefaultOrder = false;
}
BlockadeOrder::BlockadeOrder(BlockadeOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
BlockadeOrder::~BlockadeOrder() {}

//methods
bool BlockadeOrder::validate()
{
	//If there's no player owner, don't do anything.
	if (target->getPlayer() == nullptr) {
		return false;
	}
	if (CurrentPlayer == target->getPlayer())
	{
		valid = true;
	}


	return valid;
}
void BlockadeOrder::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid\n";
	else if (validate())
	{
		cout << "in BlockadeOrder execution" << endl;
		//Double the number of armies in the target territory.
		int TotalArmies = target->getArmyCount();
		TotalArmies = TotalArmies * 2;
		cout << "initialized TotalArmies (BlockadeOrder execution) to " << TotalArmies << endl;
		//Make it owned by nobody as it is technically within a blockade.
		target->setPlayer(nullptr);
		cout << "set player to null" << endl;
		target->setArmyCount(TotalArmies);
		cout << "set armyCount" << endl;
		cout << "Finished with BlockadeOrder execution" << endl;
	}
	else
	{
		cout << "Order to Blockadde is not valid\n";
	}
}

//Operators
BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& order) = default;
#pragma endregion

#pragma region AirliftOrder
//AirliftOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
AirliftOrder::AirliftOrder()
{
	type = "Airlift";
	description = "Uses planes to move troops from a destination to another in a single turn";
	effect = "Removes troops from a territory and adds that value to the existing one in another one, regardless if both territories are connected";
	valid = false;
	isDefaultOrder = true;
}

AirliftOrder::AirliftOrder(Player* CurrentPlayer, Territory* source, Territory* target, int ATA)
{
	this->CurrentPlayer = CurrentPlayer;
	this->source = source;
	this->target = target;
	ArmiesToAirlift = ATA;
	type = "Airlift";
	description = "Uses planes to move troops from a destination to another in a single turn";
	effect = "Removes troops from a territory and adds that value to the existing one in another one, regardless if both territories are connected";
	valid = false;
	isDefaultOrder = false;
}
AirliftOrder::AirliftOrder(AirliftOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
AirliftOrder::~AirliftOrder() {}

//methods
bool AirliftOrder::validate()
{
	if (CurrentPlayer == source->getPlayer() && CurrentPlayer == target->getPlayer())
	{
		valid = true;
	}


	return valid;
}
void AirliftOrder::execute()
{
	if (isDefaultOrder)
	{
		cout << "This order is default and invalid\n";
		return;
	}
	int TotalArmies;
	if (validate())
	{
		//First, move armies by setting a new amount of armies in the target
		TotalArmies = ArmiesToAirlift + target->getArmyCount();
		target->setArmyCount(TotalArmies);
		//Then, substract current number of armies in source by removing number of armies that moved
		TotalArmies = source->getArmyCount() - ArmiesToAirlift;
		source->setArmyCount(TotalArmies);
	}
	else
	{
		cout << "Order to airlift is not valid\n";
	}
}

//Operators
AirliftOrder& AirliftOrder::operator=(const AirliftOrder& order) = default;
#pragma endregion

#pragma region NegotiateOrder
//NegotiateOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
NegotiateOrder::NegotiateOrder()
{
	type = "Negotiate";
	description = "Initiates diplomacy with another player";
	effect = "Interacts with another player";
	valid = false;
	isDefaultOrder = true;
}

NegotiateOrder::NegotiateOrder(Player* CurrentPlayer, Player* TargetPlayer)
{
	this->CurrentPlayer = CurrentPlayer;
	this->TargetPlayer = TargetPlayer;
	type = "Negotiate";
	description = "Initiates diplomacy with another player";
	effect = "Interacts with another player";
	valid = false;
	isDefaultOrder = false;
}
NegotiateOrder::NegotiateOrder(NegotiateOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
NegotiateOrder::~NegotiateOrder() {}

//methods
bool NegotiateOrder::validate()
{
	if (CurrentPlayer != TargetPlayer && TargetPlayer != nullptr)
	{
		valid = true;
	}
	return valid;
}
void NegotiateOrder::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid\n";
	else if (validate())
	{
		Armistice.push_front(TargetPlayer);
	}
	else
	{
		cout << "Order to negotiate is not valid\n";
	}
}

//Operators
NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& order) = default;
#pragma endregion

std::string OrderList::stringToLog() {
	return "";
}

std::string Order::stringToLog() {
	return "";
}


#pragma region CheaterOrder
//AirliftOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
CheaterOrder::CheaterOrder()
{
	type = "Cheater";
	description = "Captures target territory without a battle";
	effect = "Changes the target territory's player to the current one";
	valid = false;
	isDefaultOrder = true;
}

CheaterOrder::CheaterOrder(Player* CurrentPlayer, Territory* target)
{
	this->CurrentPlayer = CurrentPlayer;
	this->target = target;
	type = "Cheater";
	description = "Captures target territory without a battle";
	effect = "Changes the target territory's player to the current one";
	valid = false;
	isDefaultOrder = false;
}
CheaterOrder::CheaterOrder(CheaterOrder& order)
{
	type = order.type;
	description = order.description;
	effect = order.effect;
	valid = order.valid;
}

//Deconstructor
CheaterOrder::~CheaterOrder() {}

//methods
bool CheaterOrder::validate()
{
	if (CurrentPlayer != target->getPlayer()) //First, we check if player of source territory is NOT the same as the current player
	{
		//Then, we check through the list of adjacent territories of the target and see if one is the territory of the Current player
		for (Territory* x : target->getAdjacentTerritories())
		{
			if (x->getPlayer() == CurrentPlayer)
			{
				valid = true;
			}
		}
	}
	return valid;
}
void CheaterOrder::execute()
{
	if (isDefaultOrder)
	{
		cout << "This order is default and invalid\n";
		return;
	}
	if (validate())
	{
		target->setPlayer(CurrentPlayer);
		//The player who won draws a card if they can and if they haven't already drawn any after playing orders this turn.
		if (CurrentPlayer->hand->deckPlayedWith->getNumDrawableCards() > 0 && CurrentPlayer->drewCard == false) {
			CurrentPlayer->hand->drawCard();
			GetCard = true;
			CurrentPlayer->drewCard = true;
		} else {
			cout << "Player " << CurrentPlayer->playerID << " cannot draw any more cards because there are no more cards left to draw."<<endl;
			GetCard = false;
		}
		target->getPlayer()->gotAttacked = true; //Value changed at the end of an attack order if it is executed successfully
	}
	else
	{
		cout << "Order to Cheat is not valid\n";
	}
}

//Operators
CheaterOrder& CheaterOrder::operator=(const CheaterOrder& order) = default;
#pragma endregion