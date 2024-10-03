#include "OrdersDriver.h"
#include "Orders.h"
#include <iostream>

using namespace std;

void testOrdersLists()
{
	OrderList orders;
	orders.add(new Order("Attack"));
	orders.add(new DeployOrder()); 
	orders.add(new AdvanceOrder());
	orders.add(new Order("Defend"));
	orders.add(new BlockadeOrder()); 
	orders.add(new NegotiateOrder());
	orders.add(new Order("Sleep"));
	orders.add(new BombOrder());
	orders.add(new Order("Pass"));
	orders.add(new AirliftOrder());
	
	int i = 1;
	for (Order* ord : orders.getList())
	{

		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\nExecution: " << ord->execute() << "\n";
		i++;
	}
	i = 1;

	cout << "\n2nd Test";
	orders.move(2, 6);
	cout << "\nOrder has been moved\n";
	for (Order* ord : orders.getList())
	{

		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\nExecution: " << ord->execute() << "\n";
		i++;
	}
	i = 1;

	orders.remove(4);
	cout << "\nOrder has been removed\n";
	for (Order* ord : orders.getList())
	{

		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\nExecution: " << ord->execute() << "\n";
		i++;
	}
	i = 1;
}

int main()
{
	testOrdersLists();
}