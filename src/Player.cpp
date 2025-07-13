#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include <iostream>
#include <vector>

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
        if(hand[i].isAce()){
            aces+=1;
        }
        else{
            val += hand[i].getValue();
        }
    }
    while(val+11<21 && aces!=0){
        val +=11;
        aces-=1;
    }
    if(val<21 && aces!=0){
        val+=1;
        aces-=1;
    }
    
}

void Player::showhand() const{

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
    if(deck.empty()){

    }
}



