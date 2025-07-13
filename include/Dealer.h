#ifndef DEALER_H
#define DEALER_H
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include <vector>

class Player;

class Dealer{
    std::vector <Card> Dealerhand;
    bool holeCard;
    public:
    Dealer();
    
    void initialhand(Deck &deck);
    void hit(Deck &deck);
    void revealHoleCard();
    void clearhand();
    
    bool sum17() const;
    int handvalue() const;
    bool DealerBlackjack() const;
    bool bust() const;
    
    void showhand() const;
    void showfullhand() const;
    void playDealer(Deck &deck);
    
    void RoundResult(Player &p);
    void playerWins(Player &p);
    void playerLoses(Player &p);
    void push(Player &p);
};

#endif