#include "OrdersDriver.h"
#include "Orders.h"
#include <iostream>

using namespace std;

void testOrdersLists()
{
	OrderList orders; //Creates empty list of orders

	//Adds orders to the list using Order constructor for OTHER orders and Subclasses constructors for the other valid ones 
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
	
	//Test 1: Printing all the orders
	cout << "---------------------------------------------------------------------------------\n1st Test\n";
	int i = 1; //Create a integer to enumerate orders
	for (Order* ord : orders.getList()) //Reads Order in list one by one
	{
		//Print name, description and effect of order. Execute function is also launch to see if it is valid
		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\nExecution: " << ord->execute() << "\n";
		i++;
	}
	i = 1;

	//Test 2: Move order inside list and print to see new order
	cout << "\n\n---------------------------------------------------------------------------------\n2nd Test\n";
	orders.move(2, 6);//Move method launched
	for (Order* ord : orders.getList())
	{

		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\nExecution: " << ord->execute() << "\n";
		i++;
	}
	i = 1;

	//Test 3: Remove order inside list and print to see new order
	cout << "\n\n---------------------------------------------------------------------------------\n3rd Test\n";
	orders.remove(4);//Remove method launched
	for (Order* ord : orders.getList())
	{

		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\nExecution: " << ord->execute() << "\n";
		i++;
	}
	i = 1;

	//Test 4: Call for move and remove methods using invalid parameters
	cout << "\n\n---------------------------------------------------------------------------------\n4th Test\n";
	orders.move(1, 11);
	cout << "\n";
	orders.move(-5, 13);
	cout << "\n";
	orders.move(4, 4); //This uses valid parameters, but due to both been equal, it does not do anything
	orders.remove(13);
	cout << "\n";
	orders.remove(-2);
	cout << "\nResults:\n";
	//Final result of this test should be 4 errors in total
	for (Order* ord : orders.getList()) //We print results to verify
	{

		cout << i << "." << ord->getType() << "\n";
		i++;
	}
}

int main()
{
	testOrdersLists();
}