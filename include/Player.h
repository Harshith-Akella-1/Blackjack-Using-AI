#ifndef PLAYER_H
#define PLAYER_H
#include "Card.h"
#include "Deck.h"
#include <vector>

class Player{
    std::vector <Card> hand;
    std::vector <int> ChipDenominations;
    int Bankbalance;
    bool isStanding;
    bool hasSurrendered;
    int betamt;
    public:
    Player();

    void Placebet(int denomination);
    bool hasbetamt(int denomination); //added parameter here(no parameter -> denomination)
    void initialhand(Deck &deck);
    int handvalue() const;
    void showhand() const; //?
    int totalbet() const;

    void hit(Deck &deck);
    void stand();
    void doubledown(Deck &deck);
    bool candoubledown() const;
    void split();   
    bool cansplit() const;
    void Insurance();
    void surrender();
    
    void changebet();
    void addchips(int denomination, int count = 1);
    void removechips(int denomination, int count = 1);
    void showchips() const;

    bool bust() const;
    bool hasBlackjack() const;
    bool hasSurrendered() const;
    bool isStanding() const;

    bool willcontinue();
    void newround();
    void reset();
    void Clearhand();   
};

#endif