#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Game.h"
#include <iostream>
#include <vector>


Game::Game() {
    p = Player();
    d = Dealer();
    deck = Deck();
}

void Game::showHand(int handpick){
    if(handpick == 0){
        std::cout << "Primary Hand: ";
        for(int i = 0; i < p.hand.size(); i++){
            std::cout << p.hand[i].getRankString() << p.hand[i].getSuitString() << " ";
        }
        std::cout << "Value: " << p.handvalue(0) << "\n";
    }
    else{
        std::cout << "Split Hand: ";
        for(int i = 0; i < p.Splithand.size(); i++){
            std::cout << p.Splithand[i].getRankString() << p.Splithand[i].getSuitString() << " ";
        }
        std::cout << " Value: " << p.handvalue(1) << "\n";
    }
}

void Game::Betting(){
    std::cout << "Current Bank Balance is " << p.Bankbalance << " \n";
    std::cout << "Current bet amount is " << p.betamt << " \n";
    std::cout << "Possible Denominations of the chips you can select is \n";
    std::cout << "[1, 5, 25, 100, 500, 1000, 5000, 10000] \n";
    std::cout << "Press any other key to stop betting and move to the next stage \n";
    std::cout << "You can now place your bet \n";

    int chip_picker=1;
   
    while(chip_picker!=0){
        std::cout << "Enter Denomination";
        std::cin >> chip_picker;
        switch(chip_picker){
            case 1:
                p.Placebet(1);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;
            case 5:
                p.Placebet(5);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;
            case 25:
                p.Placebet(25);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;
            case 100:
                p.Placebet(100);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;
            case 500:
                p.Placebet(500);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;
            case 1000:
                p.Placebet(1000);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;    
            case 5000:
                p.Placebet(5000);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break;
            case 10000:
                p.Placebet(10000);
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                break; 
            default:
                std::cout << "Current bet amount is  " << p.betamt <<"\n";
                std::cout << "Current bank balance is  " << p.Bankbalance <<"\n";
                std::cout << "Moving to stage 2\n";
                chip_picker=0;
                break;
        }
    }
}

void Game::Initialhands(){
    p.initialhand(deck);
    d.getinitialhand(deck);

    std::cout << "Initial hand of the player is \n";
    for(int i=0;i<2;i++){
        Card c = p.hand[i];
        std::cout << c.getRankString() << c.getSuitString() << " ";
        
    }
    std::cout << "\n";
    std::cout << "Total hand value of the player is " << p.handvalue(0) << "\n";

    std::cout << "Dealer's Upcard is " << d.Dealerhand[0].getRankString() << d.Dealerhand[0].getSuitString() << "\n";
    
}

void Game::Playeroptions(int handpick){
    std::cout << "Playing ";
    if(handpick == 0) std::cout << "Primary hand \n";
    else std::cout << "Split hand \n";
    showHand(handpick);

    std::cout << "Play your option \n";
    std::cout << "0 to Hit\n";
    std::cout << "1 to Stand\n";
    if(p.candoubledown(handpick)) std::cout << "2 to Double Down\n";
    if(handpick == 0 && p.cansplit()) std::cout << "3 to Split\n";
    if(handpick == 0 && d.Dealerhand[0].isAce()) std::cout << "4 to get Insurance\n";
    std::cout << "5 to Surrender\n";
    
    int Playeroption = 0;
    std::cin >> Playeroption;

    while(Playeroption != 6) {
        switch(Playeroption) {
            case 0: 
                p.hit(deck, handpick);
                std::cout << "Player chose to hit \n";
                std::cout << "Player received Card ";
                if(handpick==0){
                    std::cout << p.hand.back().getRankString() << p.hand.back().getSuitString() << "\n"; 
                } 
                else{
                    std::cout << p.Splithand.back().getRankString() << p.Splithand.back().getSuitString() << "\n";
                }

                if(p.bust(handpick)) {
                    std::cout << "Player busted\n";
                    if(handpick==0) p.busted=true; 
                    else p.splithandbust=true;
                    Playeroption = 6; 
                } else {
                    std::cout << "Play your option \n";
                    std::cout << "0 to Hit\n";
                    std::cout << "1 to Stand\n";
                    std::cin >> Playeroption;
                }
                break;

            case 1: 
                p.stand(handpick);
                std::cout << "Player chose to stand \n";
                Playeroption = 6;
                break;
            case 2: 
                if(p.candoubledown(handpick)) {
                    p.doubledown(deck, handpick);
                    if(handpick==0) p.diddoubledown=true; 
                    else p.didsplithanddoubledown=true;
                    //std::cout << "New card received is " << p.hand.back().getRankString() << p.hand.back().getSuitString() <<"\n";
                    std::cout << "Player's total hand value is " << p.handvalue(handpick) << "\n";
                    //std::cout << "Total bet amount now is " << p.betamt <<"\n";
                    //std::cout << "Doubled. Value: " << p.handvalue(handpick) << "\n";
                    Playeroption = 6;
                }
                break;

            case 3: 
                if(handpick == 0 && p.cansplit()) {
                    p.didsplit = true;
                    p.split(deck);
                    Playeroptions(0);
                    Playeroptions(1);
                    return; 
                }
                break;

            case 4:
                if(handpick == 0 && d.Dealerhand[0].isAce()) {
                    p.Insured = true;
                    std::cout << "Player chose to get insurance \n";
                    std::cout << "Play your option \n";
                    std::cout << "0 to Hit\n";
                    std::cout << "1 to Stand\n";
                    std::cin >> Playeroption;
                }
                break;

            case 5: 
                std::cout << "Player chose to surrender \n";
                p.hasSurrendered = true;
                Playeroption = 6; 
                break;

            default:
                std::cout << "Player chose to stand \n";
                p.stand(handpick);
                Playeroption = 6; 
                break;
        }
    }
}

void Game::DealerChance(){
    std::cout << "Dealer's Downcard is "<<d.Dealerhand.back().getRankString() << d.Dealerhand.back().getSuitString() << "\n";
    if(d.DealerBlackjack()){
        d.dBJstatus = true;
        return;
    }
    if(d.Dealerhandvalue()<17){
        std::cout << "Dealer's hand value is <17 and so Dealer will take cards \n";
        d.hit(deck);
        std::cout << "Dealer's hand value is " << d.Dealerhandvalue() << "\n";
    }
    
}

void Game::settleHand(int handpick, int bet) {
    if(handpick ==0){
        std::cout << "Result for Primary Hand \n" ;
    }
    else{
        std::cout << "Result for Split Hand \n";
    }

    int pScore = p.handvalue(handpick);
    int dScore = d.Dealerhandvalue();

    
    if (p.bust(handpick)) {
        std::cout << "Player Busts\n";
        std::cout << "Player loses "<< bet << "\n";

    }

    else if (d.Dealerbust() || pScore > dScore) {
        if (d.Dealerbust()) std::cout << "Dealer Busts! ";
        std::cout << "Player Wins and will be paid "<<2 * bet <<"\n";
        
        p.Bankbalance += (2 * bet); 
    }
    
    else if (pScore == dScore) {
        std::cout << "Player pushes at " << pScore << "\n";
        p.Bankbalance += bet; 
    }
    
    else {
        std::cout << "Dealer Wins \n";
    }
}

void Game::Result() {
    std::cout << "Player balance before payout: " << p.Bankbalance << "\n";

    if (p.hasSurrendered) {
        std::cout << "Player Surrendered, half of the bet will be returned\n";
        p.Bankbalance += (p.betamt / 2);
        return; 
    }

    if (d.dBJstatus) {
        std::cout << "Dealer has a Blackjack \n";
        if (p.Insured) {
            std::cout << "Insurance pays 2:1, Player breaks even.\n";
            p.Bankbalance += p.betamt; 
        } else {
            std::cout << "Dealer wins \n";
        }
        return; 
    }   

    if (p.pBJstatus) {
        std::cout << "Player has a Blackjack and will be paid 2:1 \n";
        p.Bankbalance += (3 * p.betamt);
        return;
    }

    settleHand(0, p.betamt);

    if (p.didsplit) {
        settleHand(1, p.Splitbet);
    }

    std::cout << "Final Bank Balance: " << p.Bankbalance << "\n";
}

void Game::closeRound(){
    p.Clearhand();
    d.clearhand();
    std::cout << "Round closed. Ready for next round.\n";
}

void Game::play(){
    
    std::cout << " Phase1: Betting\n";
    Betting();
    std::cout << "Phase2: Getting Initial Hands \n";
    Initialhands();
    std::cout << "Phase3: Player's card dealing options \n";
    Playeroptions(0);
    std::cout << "Phase4: Dealer's card dealing options \n";
    DealerChance();
    std::cout << "Phase5: Scores & Results \n";
    Result();
    std::cout << "Phase6: Closing Round \n";
    

    /*
    show the player a chip tray
    the player can click on the the chips from a specific row/ column in the chip tray as long
    as a chip from a specific denomination doesn't exceed the bank balance
    if the chip denomination is greater than the bank balance then make it grey?

    */

    //
  

   // Dealing Cards

   // Initial cards
    //p.initialhand(deck);
    //d.getinitialhand(deck);

   // give dealer & player 2 cards


   //


   //deal cards

}