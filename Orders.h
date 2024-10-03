#ifndef Order_H
#define Order_H
#include <iostream>
#include <vector>

//Order class and OrderList class forward declarations
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
std::ostream& operator << (std::ostream& os, OrderList &orderlist);
std::ostream& operator << (std::ostream& os, Order& order);


//std::ostream& operator << (std::ostream& os, DeployOrder &order);
//std::ostream& operator << (std::ostream& os, AdvanceOrder &order);
//std::ostream& operator << (std::ostream& os, BombOrder &order);
//std::ostream& operator << (std::ostream& os, BlockadeOrder &order);
//std::ostream& operator << (std::ostream& os, AirliftOrder &order);
//std::ostream& operator << (std::ostream& os, NegotiateOrder &order);

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
	OrderList& operator=(const OrderList &order);

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
	bool valid;

	//Constructors
	Order();
	Order(std::string type);
	Order(Order& order);

	//Operators
	Order& operator=(const Order& order);

	//Deconstructor
	~Order();

	//Accessors
	std::string getType();
	std::string getDescription();
	std::string getEffect();

	//Methods
	bool validate();
	std::string execute();

	//Although the class can access the string values of an order, it cannot modify them since they are static, and only change it the order type itself is changed. 
	//It is possible to modify the type and automatically redefine the description and effect, but it is inefficient, thus making more easier to simply make a NEW Order
};

class DeployOrder : public Order {
public:
	//Constructors
	DeployOrder();
	DeployOrder(DeployOrder& order);

	//Operators
	DeployOrder& operator=(const DeployOrder& order);

	//Deconstructor
	~DeployOrder();

	//methods
	bool validate();
	std::string execute();
};
class AdvanceOrder : public Order {
public:
	//Constructors
	AdvanceOrder();
	AdvanceOrder(AdvanceOrder& order);

	//Operators
	AdvanceOrder& operator=(const AdvanceOrder& order);

	//Deconstructor
	~AdvanceOrder();

	//methods
	bool validate();
	std::string execute();
};
class BombOrder : public Order {
public:
	//Constructors
	BombOrder();
	BombOrder(BombOrder& order);

	//Operators
	BombOrder& operator=(const BombOrder& order);

	//Deconstructor
	~BombOrder();

	//methods
	bool validate();
	std::string execute();
};
class BlockadeOrder : public Order {
public:
	//Constructors
	BlockadeOrder();
	BlockadeOrder(BlockadeOrder& order);

	//Operators
	BlockadeOrder& operator=(const BlockadeOrder& order);

	//Deconstructor
	~BlockadeOrder();

	//methods
	bool validate();
	std::string execute();
};
class AirliftOrder : public Order {
public:
	//Constructors
	AirliftOrder();
	AirliftOrder(AirliftOrder& order);

	//Operators
	AirliftOrder& operator=(const AirliftOrder& order);

	//Deconstructor
	~AirliftOrder();

	//methods
	bool validate();
	std::string execute();
};
class NegotiateOrder : public Order {
public:
	//Constructors
	NegotiateOrder();
	NegotiateOrder(NegotiateOrder& order);

	//Operators
	NegotiateOrder& operator=(const NegotiateOrder& order);

	//Deconstructor
	~NegotiateOrder();

	//methods
	bool validate();
	std::string execute();
};

#endif