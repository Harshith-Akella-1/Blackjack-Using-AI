#include "Card.h"
#include "Deck.h"
#include <vector>
#include <random>
#include <algorithm>


Deck::Deck(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=2;k<15;k++){
                deck.push_back(Card(static_cast<Suits>(j),static_cast<Ranks>(k)));
            }  
        }
    }
    RandomShuffle();
}


std::vector <Card>& Deck::getCards(){
    return deck;
}


bool Deck::isEmpty() const{
    return deck.empty();
}



void Deck::RandomShuffle(){
    std::random_device rd; // Random device to provide us the initial seed for Mersenne Twister
    std::mt19937_64 engine(rd()); // Mersenne Twister for better randomness
    std::shuffle(deck.begin(), deck.end(), engine);
}

       

void Deck::reset(){      
    deck.clear(); // If the player wants the deck to be reset(not required in general cases where the deck would become empty)
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=2;k<15;k++){
                deck.push_back(Card(static_cast<Suits>(j),static_cast<Ranks>(k)));
            }  
        }
    }
    RandomShuffle();
}


Card Deck::GiveCard(){
    if(isEmpty()){  // Incase of an empty deck, we can start over with a new set of decks
        reset();
    }

    Card togive = deck.back();
    deck.pop_back();
    return togive;
}
