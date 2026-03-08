#ifndef DEALER_H
#define DEALER_H
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include <vector>


class Player; //Note for later

class Dealer{
    public: 
        std::vector <Card> Dealerhand;
        bool holeCard =false;
        bool dBJstatus=false;
        // int sumofcards;
        // bool Playerwins;  //shift will do in game.cpp
        // bool Dealerwins; will do in game.cpp // shift // Need both values inorder to have functionality for push
        Dealer();
       
        void getinitialhand(Deck &deck);
        int Dealerhandvalue() const;    
        bool DealerBlackjack();
        void hit(Deck &deck);
        //void revealHoleCard(); //need GUI
        void clearhand();
        bool Dealerbust() const;


        /*
        bool sum17() const;
        
       
        void showhand() const; //need GUI
        void showfullhand() const; //need GUI
        void playDealer(Deck &deck); //?

        void RoundResult(Player &p);
        void playerWins(Player &p);
        void playerLoses(Player &p);
        void push(Player &p);
    */


};

#endif


/*
get initial cards
check blackjack status
if dbj return control to game as dbj being true and dscore=21
insurance plays etc will be taken care by game
else
hit while the sum of the cards is less than 17
once exceeds, return score
*/
// status of dealer blackjack
// total score of dealer blackjack
// upcard of dealer
// status of sum of dealer cards
// receive the status of player cards
// handling money for based on game result

