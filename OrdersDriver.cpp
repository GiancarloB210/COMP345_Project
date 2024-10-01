#include "OrdersDriver.h"

#include "Orders.h"

OrdersDriver::OrdersDriver() {

}

void OrdersDriver::testOrdersLists() {
    OrdersList orderList;
    orderList.add(new BombOrder(true));
    orderList.add(new DeployOrder(true));
    orderList.add(new AdvanceOrder(true));
    orderList.add(new BlockadeOrder(true));
    orderList.add(new AirliftOrder(true));
    orderList.add(new NegotiateOrder(true));
    //2 -> AdvanceOrder
    orderList.move(2, 4);
    for (Order* o : orderList.orders) {
        o->execute();
    }
}