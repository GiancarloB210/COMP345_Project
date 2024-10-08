
#include "Orders.h"
#include <iostream>

using namespace std;

//OrderList Class
//--------------------------------------------------------------------------------------------------------

//Constructors
OrderList::OrderList() //Default Constructor
{
	this->ListofOrders = std::vector<Order*> ();
}
OrderList::OrderList(std::vector<Order*> list) //Parameter Constructor
{
	ListofOrders = list;
}
OrderList::OrderList(OrderList &orderlist) //Copy Constructor
{
	ListofOrders = orderlist.ListofOrders;
}

//Deconstructor
OrderList::~OrderList(){}

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
std::ostream& operator << (std::ostream& os, OrderList &orderlist)
{
	std::string ListNames;
	for (int i = 0; i < orderlist.getList().size(); i++)
	{
		ListNames.append("\n\t" + (i + 1) + orderlist.getList().at(i)->getType());
	}
	return (os << "List of Orders:\n" << ListNames);
}
OrderList& OrderList::operator=(const OrderList &order) = default;

//Order Class
//--------------------------------------------------------------------------------------------------------

//Constructors
Order::Order() {} //Default Constructor (Used for subclasses)
Order::Order(std::string type)  //Parameter Constructor (Used to create other orders)
{
	this->type = type;
	description = "This order has the name " + type + " and it is unsupported";
	effect = "This order does not to anything";
	valid = false; //Every order that is not one of the subclasses will be invalid by default
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
std::string Order::execute()
{
	if (validate())
	{
		return "Nope";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
std::ostream& operator << (std::ostream& os, Order& order)
{
	return (os << "Order: " << order.getType() << "\nDescription: " << order.getDescription() << "\nEffect: " << order.getEffect());
}
Order& Order::operator=(const Order& order) = default;


//DeployOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
DeployOrder::DeployOrder()
{
	type = "Deploy";
	description = "Add a set number of troops to a location in the map";
	effect = "Adds a number to the value of present troops in a territory";
	valid = true;
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
	return valid;
}
std::string DeployOrder::execute()
{
	if (validate())
	{
		return "Deploying Troops...";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
DeployOrder& DeployOrder::operator=(const DeployOrder& order) = default;

//AdvanceOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
AdvanceOrder::AdvanceOrder()
{
	type = "Advance";
	description = "Move a group of units from a territory to the other. Battle will take place if the destination is an enemy territory or neutral territory";
	effect = "Remove a number of troops from the initial territory, then add the same number to the next territory. Battle functions will be launch the destination is not friendly";
	valid = true;
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
	return valid;
}
std::string AdvanceOrder::execute()
{
	if (validate())
	{
		return "Moving troops...";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& order) = default;

//BombOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
BombOrder::BombOrder()
{
	type = "Bomb";
	description = "Does damage to the units in the target territory";
	effect = "Removes a number of hostile troops on a selected territory";
	valid = true;
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
	return valid;
}
std::string BombOrder::execute()
{
	if (validate())
	{
		return "BOOM!!!";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
BombOrder& BombOrder::operator=(const BombOrder& order) = default;

//BlockadeOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
BlockadeOrder::BlockadeOrder()
{
	type = "Blockade";
	description = "Increase the amount of troops in a friendly territory and transforms that territory into neutral territory";
	effect = "Triples the value of troops in a owned territory and changes designation to neutral";
	valid = true;
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
	return valid;
}
std::string BlockadeOrder::execute()
{
	if (validate())
	{
		return "You.... Shall... Not.... PASS!!!";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& order) = default;

//AirliftOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
AirliftOrder::AirliftOrder()
{
	type = "Airlift";
	description = "Uses planes to move troops from a destination to another in a single turn";
	effect = "Removes troops from a territory and adds that value to the existing one in another one, regardless if both territories are connected";
	valid = true;
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
	return valid;
}
std::string AirliftOrder::execute()
{
	if (validate())
	{
		return "Airlifting troops towards target";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
AirliftOrder& AirliftOrder::operator=(const AirliftOrder& order) = default;

//NegotiateOrder Subclass
//--------------------------------------------------------------------------------------------------------

//Constructors
NegotiateOrder::NegotiateOrder()
{
	type = "Negotiate";
	description = "Initiates diplomacy with another player";
	effect = "Interacts with another player";
	valid = true;
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
	return valid;
}
std::string NegotiateOrder::execute()
{
	if (validate())
	{
		return "So uncivilised...";
	}
	else
	{
		return "Order no valid";
	}
}

//Operators
NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& order) = default;