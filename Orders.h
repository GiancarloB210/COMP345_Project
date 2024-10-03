#ifndef ORDERS_H
#define ORDERS_H
#include <cstdint>
#include <string>
#include <list>

using namespace std;

static int16_t staticOrderID = 0;

class Order {
public:
    std::string orderType;
    bool isValid;
    int16_t orderID;

    Order();
    Order(std::string orderType, bool isValid);
    Order(Order& order);
    friend ostream& operator <<(ostream& out_stream, Order& order);
    friend istream& operator >>(istream& in_stream, Order& order);
    Order& operator=(Order order);
    bool validate();
    bool execute();
};

class DeployOrder:public Order {
public:
    DeployOrder(bool isValid);
    DeployOrder(DeployOrder& order);
    DeployOrder(Order& order);
};

class AdvanceOrder:public Order {
public:
    AdvanceOrder(bool isValid);
    AdvanceOrder(AdvanceOrder& order);
    AdvanceOrder(Order& order);
};

class BombOrder:public Order {
public:
    BombOrder(bool isValid);
    BombOrder(BombOrder& order);
    BombOrder(Order& order);
};

class BlockadeOrder:public Order {
public:
    BlockadeOrder(bool isValid);
    BlockadeOrder(BlockadeOrder& order);
    BlockadeOrder(Order& order);
};

class AirliftOrder:public Order {
public:
    AirliftOrder(bool isValid);
    AirliftOrder(AirliftOrder& order);
    AirliftOrder(Order& order);
};

class NegotiateOrder:public Order {
public:
    NegotiateOrder(bool isValid);
    NegotiateOrder(NegotiateOrder& order);
    NegotiateOrder(Order& order);
};

class OtherSpecialOrder:public Order {
public:
    OtherSpecialOrder(std::string orderType, bool isValid);
    OtherSpecialOrder(OtherSpecialOrder& order);
    OtherSpecialOrder(Order& order);
    bool validate();
};

class OrdersList {
public:
    std::list<Order*> orders;

    OrdersList();
    OrdersList(std::list<Order*> orders);
    OrdersList(OrdersList& orderList);
    friend ostream& operator <<(ostream& out_stream, OrdersList& orderList);
    friend istream& operator >>(istream& in_stream, OrdersList& orderList);
    OrdersList& operator=(OrdersList order);
    void add(Order* newOrder);
    void remove(int targetPosition);
    void move(int originalIndex, int targetPosition);
};
#endif //ORDERS_H