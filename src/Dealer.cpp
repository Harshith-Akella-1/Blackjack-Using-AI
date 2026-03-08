#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include <iostream>
#include <vector>

Dealer::Dealer(){}

void Dealer::getinitialhand(Deck &deck){
    for(int i=0;i<2;i++){
        Dealerhand.push_back(deck.GiveCard());
    }
}

int Dealer::Dealerhandvalue() const{
    int val=0;
    int aces=0;
    for(int i=0;i<Dealerhand.size();i++){
        val+=Dealerhand[i].getValue();
        if(Dealerhand[i].isAce()){
            aces++;
        }
    }    
    while(val>21 && aces!=0){
        val -=10;
        aces--;
    }
    return val;
}

bool Dealer::DealerBlackjack() {
    return (Dealerhand.size() == 2 && Dealerhandvalue() ==21);
}

void Dealer::hit(Deck &deck){
    while(Dealerhandvalue()< 17){
        Dealerhand.push_back(deck.GiveCard());
        std::cout << "New Card received is " << Dealerhand.back().getRankString() << Dealerhand.back().getSuitString() <<"\n";
        
    }
}

void Dealer::clearhand(){
    Dealerhand.clear();
    dBJstatus=false;
}

bool Dealer::Dealerbust() const{
    int val =Dealerhandvalue();
    if(val>21){
        return true;
    }

   return false;
}

/*
bool Dealer::sum17() const{
    int val =Dealerhandvalue();
    if(val<17) {
        return false;
    }
    return true;
}



void Dealer::RoundResult(Player &p){
    if(!p.bust() && !Dealerbust()){

    }
    else{
        if(p.bust()){
            Dealerwins =true;
            playerLoses(p);

       }
        else{
            Playerwins =true;
            playerWins(p);
       }
    }

}

void Dealer::playerWins(Player &p){
    int profit = 1.5*betamt;
    Bankbalance += betamt;
}

void Dealer::playerLoses(Player &p){
    betamt =0;
    ChipDenominations.clear();
}

void Dealer::push(Player &p){
    //move control to gamepage again

}*/

// status of dealer blackjack
// total score of dealer blackjack
// upcard of dealer
// status of sum of dealer cards
// receive the status of player cards

// handling money for based on game result