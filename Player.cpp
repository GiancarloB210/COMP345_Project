#include "Player.h"

Player::Player() {
    this->orderList = new OrdersList();
    this->playerHand = new Hand(new Deck());
}

Player::Player(Player& player) {
    this->orderList = player.orderList;
    this->playerHand = player.playerHand;
}

Player& Player::operator=(Player player) {
    OrdersList tempOrderListPointer = *(this->orderList);
    *(this->orderList) = *(player.orderList);
    *(player.orderList) = tempOrderListPointer;

    Hand tempHandPointer = *(this->playerHand);
    *(this->playerHand) = *(player.playerHand);
    *(player.playerHand) = tempHandPointer;

    return *this;
}

void Player::issueOrder(std::string orderType) {
    if (orderType == "Bomb") {
        this->orderList->add(new BombOrder(true));
    }
    else if (orderType == "Deploy") {
        this->orderList->add(new DeployOrder(true));
    }
    else if (orderType == "Advance") {
        this->orderList->add(new AdvanceOrder(true));
    }
    else if (orderType == "Blockade") {
        this->orderList->add(new BlockadeOrder(true));
    }
    else if (orderType == "Airlift") {
       this->orderList->add(new AirliftOrder(true));
    }
    else if (orderType == "Negotiate") {
       this->orderList->add(new NegotiateOrder(true));
    }
    else if (orderType == "Reinforcement") {
       this->orderList->add(new OtherSpecialOrder("Reinforcement", true));
    }
    else if (orderType == "Diplomacy") {
       this->orderList->add(new OtherSpecialOrder("Diplomacy", true));
    }
    else {
       this->orderList->add(new OtherSpecialOrder("Invalid", false));
    }
}

void Player::playCard(int16_t targetCardID) {
    for (int i = 0;i < 40; i++) {
        if (this->playerHand->cardsInHand[i] != nullptr) {
            if (this->playerHand->cardsInHand[i]->cardID == targetCardID) {
                Order* playedOrder = this->playerHand->play(i);
                this->orderList->orders.insert(this->orderList->orders.end(), playedOrder);
                return;
            }
        }
    }
    throw std::invalid_argument("Error: The provided card ID does not exist in the current Player's hand.");
}

void Player::draw() {
    Card* drawnCard = this->playerHand->deckPlayedWith->draw();
    this->playerHand->addToHand(drawnCard);
}

