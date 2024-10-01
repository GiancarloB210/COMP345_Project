#ifndef ORDERS_H
#define ORDERS_H
#include <string>
#include <list>

static int16_t staticOrderID = 0;

class Order {
public:
    std::string orderType;
    bool isValid;
    int16_t orderID;

    Order(std::string orderType, bool isValid);
    Order(Order& order);
    Order& operator=(Order order);
    bool validate();
    bool execute();
};

class DeployOrder:public Order {
public:
    DeployOrder(bool isValid);
    DeployOrder(DeployOrder& order);
};

class AdvanceOrder:public Order {
public:
    AdvanceOrder(bool isValid);
    AdvanceOrder(AdvanceOrder& order);
};

class BombOrder:public Order {
public:
    BombOrder(bool isValid);
    BombOrder(BombOrder& order);
};

class BlockadeOrder:public Order {
public:
    BlockadeOrder(bool isValid);
    BlockadeOrder(BlockadeOrder& order);
};

class AirliftOrder:public Order {
public:
    AirliftOrder(bool isValid);
    AirliftOrder(AirliftOrder& order);
};

class NegotiateOrder:public Order {
public:
    NegotiateOrder(bool isValid);
    NegotiateOrder(NegotiateOrder& order);
};

class OtherSpecialOrder:public Order {
public:
    OtherSpecialOrder(std::string orderType, bool isValid);
    OtherSpecialOrder(OtherSpecialOrder& order);
    bool validate();
};

class OrdersList {
public:
    std::list<Order*> orders;

    OrdersList();
    OrdersList(std::list<Order*> orders);
    OrdersList(OrdersList& orderList);
    void add(Order* newOrder);
    void remove(int16_t targetOrderID);
    void move(int originalIndex, int targetPosition);
};
#endif //ORDERS_H
