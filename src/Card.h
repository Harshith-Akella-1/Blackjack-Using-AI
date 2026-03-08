#ifndef CARD_H
#define CARD_H
#include <string>


enum class Suits{ //Enum types for faster Calculations
    HEARTS, DIAMONDS, CLUBS, SPADES
};

enum class Ranks{
    TWO =2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

class Card{
    public:
        Suits suit;
        Ranks rank;
        //bool hardAce;

        Card(Suits s, Ranks r);

        Suits getSuit() const; //Enum types for Calculations
        Ranks getRank() const;

        std::string getSuitString() const; //String types for Full String Readability & representation
        std::string getRankString() const;   

        int getValue() const; // For later purposes as we don't have one for returning the card value

        bool isAce() const ;
        bool operator ==(const Card &other) const;

};

#endif