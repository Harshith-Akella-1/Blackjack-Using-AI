#ifndef PLAYER_H
#define PLAYER_H
#include "Card.h"
#include "Deck.h"
#include <vector>

class Player{
    public:
        std::vector <Card> hand;
        std::vector <int> ChipDenominations;
        int Bankbalance;
        int betamt;
        int Totalhandvalue=0;
        bool isStanding = false;
        bool hasSurrendered = false;
        bool diddoubledown = false; //maybe not needed ?
        bool didsplit = false;
        int  Splitbet=0;//fnt
        std::vector <Card> Splithand;
        std::vector <int> Splitchips; //tnt
        bool splitstatus =false;
        int handpick =0; //if 1 then changes are made to the split hand
        bool Splithandstanding =false;
        bool Splithanddoublingdown = false;
        bool didsplithanddoubledown =false;
        bool Insured = false;
        bool busted=false; 
        bool pBJstatus=false;
        bool splithandbust=false;
        
        Player();

        bool hasbetamt(int denomination); //added parameter here(no parameter -> denomination)    
        void Placebet(int denomination); //requires changes because of GUI
        void initialhand(Deck &deck);
        int handvalue(int handpick) const;
        // void showhand(int handpick) const; 
        int totalbet(int handpick) const;
        int getBetamt(int handpick);
        

        // void actionselection(); // willwork during GUI development

        void hit(Deck &deck, int handpick);
        void stand(int handpick); //will modify during GUI development as control shift needs to be registered as well
        bool candoubledown(int handpick) const; //The doubledown/cdd and split/cs operate only on hand I think, update while working on GUI
        void doubledown(Deck &deck, int handpick); // check once for requirement of calling stand function
        bool cansplit() const;
        void split(Deck &deck);  

        /*Will include the below functions in game.cpp
        //void Insurance(); //need Dealer's cards will work from dealers or game
        //void surrender();
        */

        void addchips(int denomination); // Need to change while working on GUI
        void removechips(int denomination);
        bool bust(int handpick) const;
        bool hasBlackjackf() const; // we'll return the number of blackjacks
        bool isStandingf(int handpick) const;

        /*void showchips() const; // will include after adding GUI
        void changebet(); // till here
        */
        /*

        bool willcontinue(); //will include later
        void newround();
        void restart(); // will move to different header

        */
        void Clearhand();  
};


#endif