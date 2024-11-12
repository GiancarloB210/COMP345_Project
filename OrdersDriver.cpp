#include "OrdersDriver.h"
#include "Orders.h"
#include "Map.h"
#include <iostream>

using namespace std;


void testOrdersExecution()
{
	OrderList orders; //Creates empty list of orders

	//Create lists of adjacent territories
	std::list<std::string> t1Adj = { "t4", "t5", "t2" };
	std::list<std::string> t2Adj = { "t1", "t6" };
	std::list<std::string> t3Adj = { "t6" };
	std::list<std::string> t4Adj = { "t1", "t8" };
	std::list<std::string> t5Adj = { "t1", "t7" };
	std::list<std::string> t6Adj = { "t2", "t3", "t7", "t9" };
	std::list<std::string> t7Adj = { "t5", "t6", "t8" };
	std::list<std::string> t8Adj = { "t4", "t7", "t10" };
	std::list<std::string> t9Adj = { "t6", "t10" };
	std::list<std::string> t10Adj = { "t9", "t8" };

	Territory* t1;
	Territory* t2;
	Territory* t3;
	Territory* t4;
	Territory* t5;
	Territory* t6;
	Territory* t7;
	Territory* t8;
	Territory* t9;
	Territory* t10;

	//We create territories to use
	t1 = new Territory("t1", "Tutuland", 0, 0, t1Adj);
	t2 = new Territory("t2", "Tutuland", 1, 0, t2Adj);
	t3 = new Territory("t3", "Tutuland", 2, 0, t3Adj);
	t4 = new Territory("t4", "Tutuland", 0, 1, t4Adj);
	t5 = new Territory("t5", "Tutuland", 1, 1, t5Adj);
	t6 = new Territory("t6", "Tutuland", 2, 1, t6Adj);
	t7 = new Territory("t7", "Tutuland", 0, 2, t7Adj);
	t8 = new Territory("t8", "Tutuland", 1, 2, t8Adj);
	t9 = new Territory("t9", "Tutuland", 2, 2, t9Adj);
	t10 = new Territory("t10", "Tutuland", 0, 3, t10Adj);

	//Setting to each territory, a list of adjacent territories
	t1->setAdjacentTerritories({t2, t4, t5});
	t2->setAdjacentTerritories({t1, t6});
	t3->setAdjacentTerritories({t6});
	t4->setAdjacentTerritories({t1, t8});
	t5->setAdjacentTerritories({t1, t7});
	t6->setAdjacentTerritories({t2, t3, t7, t9});
	t7->setAdjacentTerritories({t5, t6, t8});
	t8->setAdjacentTerritories({t4, t7, t10});
	t9->setAdjacentTerritories({t6, t10});
	t10->setAdjacentTerritories({t8, t9});

	//We create a list of territories (2 to have one for each player)
	std::list<Territory*> territoriesList1 = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10 };
	std::list<Territory*> territoriesList2 = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10 };

	//We create players and a hand for them
	Hand* hd = new Hand();
	Player* P1 = new Player(&territoriesList1, hd);
	Player* P2 = new Player(&territoriesList2, hd);

	//We set players for each territory
	t1->setPlayer(P1);
	t2->setPlayer(P1);
	t3->setPlayer(P1);
	t4->setPlayer(P1);
	t5->setPlayer(P1);
	t6->setPlayer(P2);
	t7->setPlayer(P2);
	t8->setPlayer(P2);
	t9->setPlayer(P2);
	t10->setPlayer(P2);

	//We set amount of armies in each territory
	t1->setArmyCount(3);
	t2->setArmyCount(4);
	t3->setArmyCount(2);
	t4->setArmyCount(2);
	t5->setArmyCount(5);
	t6->setArmyCount(5);
	t7->setArmyCount(6);
	t8->setArmyCount(2);
	t9->setArmyCount(4);
	t10->setArmyCount(3);


	std::list<Territory*> Continent = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10 };


	//Adds orders to the list using Orders constructors
	orders.add(new DeployOrder(P1, t5, 8)); //We, as Player 1, add 8 armies to territory 5
	orders.add(new AdvanceOrder(P1, t1, t5, 2)); //We, as Player 1, move 2 armies from territory 1 to territory 5
	orders.add(new BlockadeOrder(P1, t4)); //We, as Player 1, blockade territory 4 
	orders.add(new BombOrder(P1, t7)); //We, as Player 1, bomb territory 7
	orders.add(new AdvanceOrder(P1, t5, t7, 15)); //We, as Player 1, move 15 armies from territory 5 to territory 7, and attack
	orders.add(new NegotiateOrder(P1, P2)); //We negitate an armistice with Player 2
	orders.add(new AdvanceOrder(P1, t2, t6, 4)); //We, as Player 1, move 2 armies from territory 2 to territory 6, and attack (Should be invalid due to armistice)
	orders.add(new AirliftOrder(P1, t2, t3, 4)); //We, as Player 1, airlift 4 armies from territory 2 to territory 3

	//Test 1: Printing all the orders and execute
	cout << "---------------------------------------------------------------------------------\n1st Test\n";
	int territoryID = 1;
	//Prints beginning state of territories
	cout << "----------------------------------------------------------Territory At the Beginning--------------------------------------------------------------\n";
	cout << "Current Player --> Player 1 \n";
	for (Territory* t : Continent)
	{
		cout << "Territory #" << territoryID << " --> Owner: ";
		if (t->getPlayer() == P1)
			cout << "Player 1";
		else if (t->getPlayer() == P2)
			cout << "Player 2";
		else if (t->getPlayer() == nullptr)
			cout << "Neutral";
		cout << " || # of Armies in Territory: " << t->getArmyCount() << "\n";
		territoryID += 1;
	}
	cout << "Current Armistice: |";
	for (Player* p : orders.getArmistice())
		if (p == P2)
			cout << "Player 2";
	cout << "| \n";

	if (orders.isGettingCard())
		cout << "Player IS getting a card at the end of turn\n";
	else if (!orders.isGettingCard())
		cout << "Player IS NOT getting a card at the end of turn\n";
	cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n";

	int i = 1; //Create a integer to enumerate orders
	for (Order* ord : orders.getList()) //Reads Order in list one by one
	{
		//Print name, description and effect of order. Execute function is also launch to see if it is valid
		cout << i << "." << ord->getType() << "\nDescription: " << ord->getDescription() << "\nEffect: " << ord->getEffect() << "\n Executing... \n";
		ord->execute();
		cout << "Order Executed successfully\n\n";
		i++;

		territoryID = 1;
		cout << "----------------------------------------------------------Territory Changes after " << ord->getType() << " Order----------------------------------------------------------------------\n";
		cout << "Current Player --> Player 1 \n";
		for (Territory* t : Continent)
		{
			cout << "Territory #" << territoryID << " --> Owner: ";
			if (t->getPlayer() == P1)
				cout << "Player 1";
			else if (t->getPlayer() == P2)
				cout << "Player 2";
			else if (t->getPlayer() == nullptr)
				cout << "Neutral";
			cout << " || # of Armies in Territory: " << t->getArmyCount() << "\n";
			territoryID += 1;
		}
		cout << "Current Armistice: |";
		for (Player* p : orders.getArmistice())
			if (p == P2)
				cout << "Player 2";
		cout << "| \n";

		if (orders.isGettingCard())
			cout << "Player IS getting a card at the end of turn\n";
		else if (!orders.isGettingCard())
			cout << "Player IS NOT getting a card at the end of turn\n";
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	}
	i = 1;








	/*
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
	*/

	cout << "Now deleting pointers and values to avoid memory leak.... \n\n";
	int id = 1;
	for (Order* ord : orders.getList()) //Reads Order in list one by one
	{
		delete ord;
		cout << "Delete #" << id << " executed successfully\n";
		id += 1;
	}
	

	delete P1;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete P2;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete hd;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;

	/*delete t1;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t2;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t3;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t4;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t5;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t6;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t7;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t8;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t9;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;
	delete t10;
	cout << "Delete #" << id << " executed successfully\n";
	id += 1;*/


	id = 1;
	P1 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	P2 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	hd = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t1 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t2 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t3 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1; 
	t4 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t5 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t6 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t7 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t8 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t9 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;
	t10 = NULL;
	cout << "NULL #" << id << " executed successfully\n";
	id += 1;

}

 int main()
 {
 	testOrdersExecution();
	 cout << "\n\n\n\n Test Finish";
	 return 0;
 }