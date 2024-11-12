
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
Order::Order() { isDefaultOrder = true;} //Default Constructor (Used for subclasses)
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

//methods
bool DeployOrder::validate()
{
	if (CurrentPlayer == target->getPlayer()) //Verify that current player is the same as territory owner
		valid = true;

	return valid;
}
void DeployOrder::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid";
	else if (validate())
	{
		int TotalArmies = ArmiesToAdd + target->getArmyCount();
		target->setArmyCount(TotalArmies);
	}
	else
	{
		cout << "Deploy Order is not valid";
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
	if (CurrentPlayer == source->getPlayer()) //First, we check if player of source territory is the same as the current player
	{
		//Then, we check through the list of adjacent territories of the source and see if one is equal to the target
		for (std::string x : source->getAdjacentTerritoryNames())
		{
			if (target->getName() == x)
			{
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
		cout << "This order is default and invalid";
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
			//First, move armies by setting a new amount of armies in the target
			TotalArmies = ArmiesToAdvance + target->getArmyCount();
			target->setArmyCount(TotalArmies);
			//Then, substract current number of armies in source by removing number of armies that moved
			TotalArmies = source->getArmyCount() - ArmiesToAdvance;
			source->setArmyCount(TotalArmies);
		}
		else
		{
			//We check if we have an agreement with the current holder of the target territory
			for (Player* p : Armistice)
			{
				if (target->getPlayer() == p)
				{
					cout << "Order is invalid since we are attacking a player that has a not agression pact with us this turn";
					return;
				}
			}
			//First, we substract current number of armies in source by removing number of armies that moved
			TotalArmies = source->getArmyCount() - ArmiesToAdvance;
			source->setArmyCount(TotalArmies);

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
						TargetArmies -= 1;

					randomValue = distrib(gen);
					if (randomValue <= 7)
						ArmiesToAdvance -= 1;
				}
			}

			//After battle, we see if attacker won and has troups left. Else, nothing happens
			if (TargetArmies == 0 || ArmiesToAdvance != 0)
			{
				target->setArmyCount(ArmiesToAdvance);
				target->setPlayer(CurrentPlayer);
				GetCard = true;
			}

		}
	}
	else
	{
		cout << "Order to Advance is not valid";
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
		//Then, we check through the list of adjacent territories of the taret and see if one is the territory of the Current player
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
		cout << "This order is default and invalid";
	else if (validate())
	{
		//We check if we have an agreement with the current holder of the target territory
		for (Player* p : Armistice)
		{
			if (target->getPlayer() == p)
			{
				cout << "Order is invalid since we are attacking a player that has a not agression pact with us this turn";
				return;
			}
		}

		int TotalArmy = target->getArmyCount();
		TotalArmy = TotalArmy / 2;
		target->setArmyCount(TotalArmy);
	}
	else
	{
		cout << "Order to bomb is not valid";
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
	if (CurrentPlayer == target->getPlayer())
	{
		valid = true;
	}


	return valid;
}
void BlockadeOrder::execute()
{
	if (isDefaultOrder)
		cout << "This order is default and invalid";
	else if (validate())
	{
		int TotalArmies = target->getArmyCount();
		TotalArmies = TotalArmies * 2;
		target->setPlayer(nullptr);
		target->setArmyCount(TotalArmies);
	}
	else
	{
		cout << "Order to Blockadde is not valid";
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
		cout << "This order is default and invalid";
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
		cout << "Order to airlift is not valid";
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
		cout << "This order is default and invalid";
	else if (validate())
	{
		Armistice.push_front(TargetPlayer);
	}
	else
	{
		cout << "Order to negotiate is not valid";
	}
}

//Operators
NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& order) = default;
#pragma endregion