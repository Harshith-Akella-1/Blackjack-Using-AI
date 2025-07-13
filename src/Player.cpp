#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <iterator>
Player::Player(){
    Bankbalance = 20000;
}

void Player::Placebet(int denomination){
    if(hasbetamt(denomination)){
    Bankbalance -= denomination;
    ChipDenominations.push_back(denomination);  
    }
    std::cout << "insufficient balance";
    return;
}

bool Player::hasbetamt(int denomination) {
    if(denomination > Bankbalance){
        return false;
    }
    betamt += denomination;
    return true;
}

void Player::initialhand(Deck &deck){
    for(int i=0;i<2;i++){
        Player::hand.push_back(deck.getCards().back());
        deck.getCards().pop_back();
    }
}

int Player::handvalue() const{
    int val =0;
    int aces =0;
    for(int i=0;i<hand.size();i++){
        if(hand[i].isAce()){ // So initially all aces are valued at 11 while also keeping track of number of aces
            aces+=11;
        }
        else{
            val += hand[i].getValue();
        }
    }
    while(val>21 && aces!=0){ //Once the score exceeds 21, we bust so we can start considering aces as 1 from here on
        val -=10;             //
        aces--;
    }
    return val;
}

void Player::showhand() const{ //check later
    
}

int Player::totalbet() const{
    return betamt;
}

/*int Player::totalbet() const{
    int amount=0;
    for(int i=0;i<ChipDenominations.size();i++){
        amount += ChipDenominations[i];
    }
    return amount;
}*/

void Player::hit(Deck &deck){
    if(deck.isEmpty()){
        deck.reset();
    }
    Player::hand.push_back(deck.getCards().back());
    deck.getCards().pop_back();
}

void Player::addchips(int denomination){
    if(hasbetamt(denomination)){
        ChipDenominations.push_back(denomination);
        Bankbalance -= denomination;
        betamt +=denomination;
    }
    std::cout << "not sufficient money";
}

void Player::removechips(int denomination){
    auto it = std::find(ChipDenominations.begin(), ChipDenominations.end(), denomination);
    if (it != ChipDenominations.end()) {
        ChipDenominations.erase(it);
    }
    Bankbalance += denomination;
    betamt -=denomination;
}

