#include "Orders.h"

#include <iostream>
#include <ostream>
using namespace std;

Order::Order() {
    this->orderID = staticOrderID;
    staticOrderID++;
    this->orderType = "No Type";
    this->isValid = false;
}

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

Order& Order::operator=(Order order) {
    swap(this->orderID, order.orderID);
    swap(this->orderType, order.orderType);
    swap(this->isValid, order.isValid);
    return *this;
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

DeployOrder::DeployOrder(Order& order): Order(order.orderType, order.isValid) {
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

AdvanceOrder::AdvanceOrder(Order& order): Order(order.orderType, order.isValid) {
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

BombOrder::BombOrder(Order& order): Order(order.orderType, order.isValid) {
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

BlockadeOrder::BlockadeOrder(Order& order): Order(order.orderType, order.isValid) {
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

AirliftOrder::AirliftOrder(Order& order): Order(order.orderType, order.isValid) {
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

NegotiateOrder::NegotiateOrder(Order& order): Order(order.orderType, order.isValid) {
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

OtherSpecialOrder::OtherSpecialOrder(Order& order): Order(order.orderType, order.isValid) {
    this->orderType = order.orderType;
    this->isValid = order.isValid;
}

bool OtherSpecialOrder::validate() {
    if (this->isValid == true && (this->orderType == "Reinforcement" || this->orderType == "Democracy")) {
        return true;
    }
    return false;
}

ostream& operator << (ostream &out_stream, Order &order) {
    out_stream<<"Order ID: "<<order.orderID<<" | Type: "<<order.orderType<<" | Validity: "<<(order.isValid ? "Valid" : "Invalid")<<endl;
    return out_stream;
}

istream& operator >> (istream &in_stream, Order &order) {
    cout<<"What type of order would you like to create?"<<endl;
    string typeInput;
    in_stream>>typeInput;
    for (char& ch : typeInput) {
        ch = tolower(ch, locale());
    }
    if (typeInput == "deploy") {
        order.orderType = "Deploy";
        DeployOrder d_order(order);
        order = d_order;
    }
    else if (typeInput == "advance") {
        order.orderType = "Advance";
        AdvanceOrder ad_order(order);
        order = ad_order;
    }
    else if (typeInput == "bomb") {
        order.orderType = "Bomb";
        BombOrder bo_order(order);
        order = bo_order;
    }
    else if (typeInput == "blockade") {
        order.orderType = "Blockade";
        BlockadeOrder bl_order(order);
        order = bl_order;
    }
    else if (typeInput == "airlift") {
        order.orderType = "Airlift";
        AirliftOrder air_order(order);
        order = air_order;
    }
    else if (typeInput == "negotiate") {
        order.orderType = "Negotiate";
        NegotiateOrder n_order(order);
        order = n_order;
    }
    else {
        throw std::invalid_argument("Invalid card type entered");
    }
    cout<<"Is this order valid (Yes/No)?"<<endl;
    string validInput;
    in_stream>>validInput;
    for (char& ch : validInput) {
        ch = tolower(ch, locale());
    }
    if (validInput == "yes") {
        order.isValid = true;
    }
    else if (validInput == "no") {
        order.isValid = false;
    } else {
        throw std::invalid_argument("Invalid validity value entered");
    }
    return in_stream;
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

OrdersList& OrdersList::operator=(OrdersList order) {
    swap(this->orders, order.orders);
    return *this;
}

ostream& operator << (ostream &out_stream, OrdersList &orderList) {
    cout<<"Order List Contents:"<<endl;
    for (Order* o : orderList.orders) {
        if (o != nullptr) {
            cout<<*o<<endl;
        }
    }
    return out_stream;
}

istream& operator >> (istream &in_stream, OrdersList &orderList) {
    list<Order*> newOrders;
    cout<<"How many orders would you like to add to the list?"<<endl;
    int numOrders;
    in_stream>>numOrders;
    for (int i = 0;i < numOrders;i++) {
        Order *order;
        in_stream>>*order;
        newOrders.push_back(order);
    }
    orderList.orders = newOrders;
    return in_stream;
}

void OrdersList::add(Order* newOrder) {
    this->orders.insert(this->orders.end(), newOrder);
}

void OrdersList::remove(int targetPosition) {
    int tracker = 0;
    for (Order* o : this->orders) {
        if (tracker == targetPosition) {
            this->orders.remove(o);
            return;
        }
        tracker++;
    }
    throw std::invalid_argument("Error: The provided order index is invalid or not reachable in the list.");
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