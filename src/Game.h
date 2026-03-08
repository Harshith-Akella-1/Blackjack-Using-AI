#ifndef GAME_H
#define GAME_H
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include <vector>


class Game{
    public:
        Player p;
        Dealer d;
        Deck deck;

        int handpick;

        Game();

       void showHand(int handpick);

        void Betting();
        void Initialhands();
        void Playeroptions(int handpick);
        void DealerChance();
        void settleHand(int handpick, int bet);
        void Result();
        void closeRound();

        void play();




};


#endif