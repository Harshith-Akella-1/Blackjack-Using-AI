#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


Player::Player(){
    Bankbalance = 50000;
    betamt =0;
}

bool Player::hasbetamt(int denomination) {
    return (denomination <= Bankbalance);
}

void Player::Placebet(int denomination){
    if(hasbetamt(denomination)){
    Bankbalance -= denomination;
    ChipDenominations.push_back(denomination);
    betamt += denomination;
    return;
    }
    std::cout << "insufficient balance \n";
}

void Player::initialhand(Deck &deck){
    for(int i=0;i<2;i++){
        hand.push_back(deck.GiveCard());
    }
}

int Player::handvalue(int handpick) const{ //change in functionlity
    if(handpick == 0){
        int val =0;
        int aces =0;
        for(int i=0;i<hand.size();i++){
            val += hand[i].getValue();
            if(hand[i].isAce()){ // So initially all aces are valued at 11 while also keeping track of number of aces
            aces++;
            }
        }
        while(val>21 && aces!=0){
            val -=10;            
            aces--;
        }
        return val;
    }
    else{
        int val =0;
        int aces =0;
        for(int i=0;i<Splithand.size();i++){
            val += Splithand[i].getValue();
            if(Splithand[i].isAce()){ // So initially all aces are valued at 11 while also keeping track of number of aces
                aces++;
            }
        }
        while(val>21 && aces!=0){
            val -=10;            
            aces--;
        }
        return val;
    }
   
}

int Player::totalbet(int handpick) const{
    if(handpick == 0) return betamt;
    else return Splitbet;
}

int Player::getBetamt(int handpick){
    if(handpick == 0){
        return betamt;
    }
    else{
        return Splitbet;
    }
}

/*int Player::totalbet() const{
    int amount=0;
    for(int i=0;i<ChipDenominations.size();i++){
        amount += ChipDenominations[i];
    }
    return amount;
}*/

void Player::hit(Deck &deck, int handpick){
    if(deck.isEmpty()){
        deck.reset();
    }
    if(handpick == 0){
        hand.push_back(deck.GiveCard());
    }
    else {
        Splithand.push_back(deck.GiveCard());
    }
}

void Player::stand(int handpick) {
    if(handpick == 0){
        isStanding = true;
    }
    else {
        Splithandstanding = true;
    }
   
    //return control to game.cpp
}

bool Player::candoubledown(int handpick) const{
    if(handpick == 0){
        if((hand.size() == 2) && !(diddoubledown)){
            return betamt <= Bankbalance;
        }
    }
    else{
        if((Splithand.size() == 2) && !(didsplithanddoubledown)){
            return Splitbet <= Bankbalance;
        }
    }
    
    return false;
}

void Player::doubledown(Deck &deck, int handpick){
    if(handpick == 0){
        diddoubledown =true;
        Bankbalance -= betamt;
        betamt += betamt;
        int chips = ChipDenominations.size();
        for(int i=0;i<chips;i++){
            ChipDenominations.push_back(ChipDenominations[i]);
        }
        hit(deck, handpick);
        stand(handpick);
    }
    else{
        didsplithanddoubledown =true;
        Bankbalance -= Splitbet;
        Splitbet += Splitbet;
        int chips = Splitchips.size();
        for(int i=0;i<chips;i++){
            Splitchips.push_back(Splitchips[i]);
        }
        hit(deck, handpick);
        stand(handpick);
    }
}

bool Player::cansplit() const{
    if((hand.size() == 2)&&(!(betamt > Bankbalance))){
        if(hand[0].getRankString() == hand[1].getRankString()){ // juts get value wouldn't work here because 10/J/Q/K all are of same value/rank so we need string formatted response as that would be unique
            return true;
        }
    }
    return false;
}

void Player::split(Deck &deck){
    if(cansplit()){
        didsplit =true;
        Splitbet = betamt;
        Bankbalance -=betamt;
        for(int i=0;i<ChipDenominations.size();i++){
            Splitchips.push_back(ChipDenominations[i]);
        }
        Splithand.push_back(hand.back());
        hand.pop_back();
        hand.push_back(deck.GiveCard());
        Splithand.push_back(deck.GiveCard());
        splitstatus = true;
    }
}

void Player::addchips(int denomination){
    if(hasbetamt(denomination)){
        ChipDenominations.push_back(denomination);
        Bankbalance -= denomination;
        betamt +=denomination;
        return;
    }
    std::cout << "Inadequate Capital 🗿";
}

void Player::removechips(int denomination){
    auto it = std::find(ChipDenominations.begin(), ChipDenominations.end(), denomination);
    if (it != ChipDenominations.end()) {
        ChipDenominations.erase(it);
        Bankbalance += denomination;
        betamt -=denomination;
    }
}

bool Player::bust(int handpick) const{
    if(handpick == 0){
        if(handvalue(handpick) > 21){
            return true;
        }
        return false;
    }
    else{
        if(handvalue(handpick) > 21){
            return true;
        }
        return false;
    }
   
}
/*
int Player::hasBlackjackf() const{
    // for possibility of blackjack in both regular hand and split hand
    if((Splithand.size() == 2)){
        int sum=0;

       for(int i=0;i<2;i++) {
            sum += Splithand[i].getValue();
       }
        if(sum == 21){

           if((hand.size() == 2) &&(handvalue() == 21)){
                return 2;
            }  
            return 1;
        }
    }

    if((hand.size() == 2) &&(handvalue() == 21)){
        return 1;
    }

    return 0;
}*/

bool Player::hasBlackjackf() const{
    if(didsplit) return false;
    if((hand.size() == 2) &&(handvalue(0) == 21)){
       return true;
    }
    return false;
}

bool Player::isStandingf(int handpick) const{
    if(handpick == 0)   return isStanding;
    else return Splithandstanding;
}

void Player::Clearhand(){
    hand.clear();
    ChipDenominations.clear();
    isStanding = false;
    hasSurrendered =false;
    diddoubledown = false;
    betamt=0;
    busted=false;
    pBJstatus=false;
    Insured=false;

    if(didsplit){
        didsplit = false;
        Splitbet=0;
        Splithand.clear();
        Splitchips.clear();
        splitstatus =false;
        handpick =0; 
        Splithandstanding =false;
        Splithanddoublingdown = false;
        didsplithanddoubledown =false;
    }

}


