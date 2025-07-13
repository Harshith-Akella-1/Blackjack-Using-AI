#ifndef DECK_H
#define DECK_H
#include "Card.h"
#include <vector>

class Deck{
    std::vector <Card> deck;
    public:
        Deck();
        std::vector <Card>& getCards(); // DiscoVery

        bool isEmpty() const;

        void RandomShuffle();
        void reset();
        Card GiveCard();
                
};







#endif