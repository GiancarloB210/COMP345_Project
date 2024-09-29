#include "Orders.h"

#include <iostream>
#include <ostream>
using namespace std;

Order::Order(std::string orderType, bool isValid) {
    this->orderID = staticOrderID;
    staticOrderID++;
    this->orderType = orderType;
    this->isValid = isValid;
}

Order::Order(Order& order) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

bool Order::validate() {
    return this->isValid;
}

bool Order::execute() {
    if (this->validate() == true) {
        cout<<"Order of type "<<this->orderType<<" validated & executed."<<endl;
    }
}

DeployOrder::DeployOrder(bool isValid): Order("Deploy", isValid) {
    this->orderType = "Deploy";
    this->isValid = isValid;
}

DeployOrder::DeployOrder(DeployOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

AdvanceOrder::AdvanceOrder(bool isValid): Order("Advance", isValid) {
    this->orderType = "Advance";
    this->isValid = isValid;
}

AdvanceOrder::AdvanceOrder(AdvanceOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

BombOrder::BombOrder(bool isValid): Order("Bomb", isValid) {
    this->orderType = "Bomb";
    this->isValid = isValid;
}

BombOrder::BombOrder(BombOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

BlockadeOrder::BlockadeOrder(bool isValid): Order("Blockade", isValid) {
    this->orderType = "Blockade";
    this->isValid = isValid;
}

BlockadeOrder::BlockadeOrder(BlockadeOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

AirliftOrder::AirliftOrder(bool isValid): Order("Airlift", isValid) {
    this->orderType = "Airlift";
    this->isValid = isValid;
}

AirliftOrder::AirliftOrder(AirliftOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

NegotiateOrder::NegotiateOrder(bool isValid): Order("Negotiate", isValid) {
    this->orderType = "Negotiate";
    this->isValid = isValid;
}

NegotiateOrder::NegotiateOrder(NegotiateOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

OtherSpecialOrder::OtherSpecialOrder(std::string orderType, bool isValid): Order(orderType, isValid) {
    this->orderType = orderType;
    this->isValid = isValid;
}

OtherSpecialOrder::OtherSpecialOrder(OtherSpecialOrder& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

bool OtherSpecialOrder::validate() {
    if (this->isValid == true && (this->orderType == "Reinforcement" || this->orderType == "Democracy")) {
        return true;
    }
    return false;
}

OrdersList::OrdersList() {
    this->orders = std::list<Order*>{};
}

OrdersList::OrdersList(std::list<Order*> orders) {
    this->orders = orders;
}

OrdersList::OrdersList(OrdersList& orderList) {
    this->orders = orderList.orders;
}

void OrdersList::add(Order* newOrder) {
    this->orders.insert(this->orders.end(), newOrder);
}

void OrdersList::remove(int16_t targetOrderID) {
    for (Order* o : this->orders) {
        if (o->orderID == targetOrderID) {
            this->orders.remove(o);
            break;
        }
    }
    throw std::invalid_argument("Error: The provided order ID does not exist in the current OrdersList.");
}

void OrdersList::move(int originalIndex, int targetPosition) {
    Order* tempPointer = nullptr;
    int tracker = 0;
    for (Order* o : this->orders) {
        if (tracker == originalIndex) {
            Order* tempOrder(o);
            tempPointer = tempOrder;
            this->orders.remove(o);
            list<Order*>::iterator listIterator = this->orders.begin();
            advance(listIterator, targetPosition);
            this->orders.insert(listIterator, tempPointer);
            return;
        }
        tracker++;
    }
    throw std::invalid_argument("Error: The provided order ID does not exist in the current OrdersList.");
}

