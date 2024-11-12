#ifndef Order_H
#define Order_H
#include <iostream>
#include <vector>
#include <random>
#include <list>

// #include "Map.h"
// #include "Player.h"

//using namespace std;

//Order class and OrderList class forward declarations
class Player;
class Territory;
class Order;
class OrderList;

//Subclasses of Order class forward declarations
class DeployOrder;
class AdvanceOrder;
class BombOrder;
class BlockadeOrder;
class AirliftOrder;
class NegotiateOrder;

//Free functions
std::ostream& operator << (std::ostream& os, OrderList& orderlist);
std::ostream& operator << (std::ostream& os, Order& order);

//Global Variables
// bool GetCard = false;
// std::list<Player*> Armistice;

class OrderList
{
private:
	std::vector<Order*> ListofOrders;

public:
	//Constructors
	OrderList();
	OrderList(std::vector<Order*> list);
	OrderList(OrderList& orderlist);

	//Operators
	OrderList& operator=(const OrderList& order);

	//Deconstructor
	~OrderList();

	//Accessors
	std::vector<Order*> getList();

	//methods
	void add(Order* order); //This method allows us to add an other to the list

	void move(int iPos, int fPos); //This method will ask for the order's position and the position to where the order wishes to be moved to

	void remove(int Pos); //This method asks for the position of the order to remove
};

class Order
{
public:
	std::string type, description, effect;
	Player* CurrentPlayer;
	bool valid;
	bool isDefaultOrder;

	//Constructors
	Order();
	Order(Player* CurrentPlayer, std::string type);
	Order(Order& order);

	//Operators
	Order& operator=(const Order& order);

	//Deconstructor
	virtual ~Order();

	//Accessors
	std::string getType();
	std::string getDescription();
	std::string getEffect();

	//Methods
	virtual bool validate();
	virtual void execute();

	//Although the class can access the string values of an order, it cannot modify them since they are static, and only change it the order type itself is changed. 
	//It is possible to modify the type and automatically redefine the description and effect, but it is inefficient, thus making more easier to simply make a NEW Order
};

class DeployOrder : public Order {
private:
	Territory* target;
	int ArmiesToAdd;

public:
	//Constructors
	DeployOrder();
	DeployOrder(Player* CurrentPlayer, Territory* target, int ATD);
	DeployOrder(DeployOrder& order);

	//Operators
	DeployOrder& operator=(const DeployOrder& order);

	//Deconstructor
	~DeployOrder();

	//methods
	bool validate() override;
	void execute() override;
};
class AdvanceOrder : public Order {
private:
	Territory* source;
	Territory* target;
	int ArmiesToAdvance;
public:
	//Constructors
	AdvanceOrder();
	AdvanceOrder(Player* CurrentPlayer, Territory* source, Territory* target, int ATA);
	AdvanceOrder(AdvanceOrder& order);

	//Operators
	AdvanceOrder& operator=(const AdvanceOrder& order);

	//Deconstructor
	~AdvanceOrder();

	//methods
	bool validate() override;
	void execute() override;
};
class BombOrder : public Order {
private:
	Territory* target;
public:
	//Constructors
	BombOrder();
	BombOrder(Player* CurrentPlayer, Territory* target);
	BombOrder(BombOrder& order);

	//Operators
	BombOrder& operator=(const BombOrder& order);

	//Deconstructor
	~BombOrder();

	//methods
	bool validate() override;
	void execute() override;
};
class BlockadeOrder : public Order {
private:
	Territory* target;
public:
	//Constructors
	BlockadeOrder();
	BlockadeOrder(Player* CurrentPlayer, Territory* target);
	BlockadeOrder(BlockadeOrder& order);

	//Operators
	BlockadeOrder& operator=(const BlockadeOrder& order);

	//Deconstructor
	~BlockadeOrder();

	//methods
	bool validate() override;
	void execute() override;
};
class AirliftOrder : public Order {
private:
	Territory* source;
	Territory* target;
	int ArmiesToAirlift;
public:
	//Constructors
	AirliftOrder();
	AirliftOrder(Player* CurrentPlayer, Territory* source, Territory* target, int ATA);
	AirliftOrder(AirliftOrder& order);

	//Operators
	AirliftOrder& operator=(const AirliftOrder& order);

	//Deconstructor
	~AirliftOrder();

	//methods
	bool validate() override;
	void execute() override;
};
class NegotiateOrder : public Order {
private:
	Player* TargetPlayer;
public:
	//Constructors
	NegotiateOrder();
	NegotiateOrder(Player* CurrentPlayer, Player* TargetPlayer);
	NegotiateOrder(NegotiateOrder& order);

	//Operators
	NegotiateOrder& operator=(const NegotiateOrder& order);

	//Deconstructor
	~NegotiateOrder();

	//methods
	bool validate() override;
	void execute() override;
};

#endif
