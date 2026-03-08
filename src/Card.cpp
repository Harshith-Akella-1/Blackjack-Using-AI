#include "Card.h"
#include <string>
using namespace std;


Card::Card(Suits s,Ranks r){
    suit = s;
    rank = r;
}


Suits Card::getSuit() const{
    return suit;
}


Ranks Card::getRank() const{
    return rank;
}


string Card::getSuitString() const{
    switch(suit){
        case Suits::HEARTS:
            return "♥";
        case Suits::DIAMONDS:
            return "♦";
        case Suits::CLUBS:
            return "♣";
        case Suits::SPADES:
            return "♠";
    }
    return ""; //Empty string incase of an anamoly

}


string Card::getRankString() const{
    switch(rank){
        case Ranks::TWO:
           return "2";
        case Ranks::THREE:
            return "3";
       case Ranks::FOUR:
            return "4";
        case Ranks::FIVE:
            return "5";
        case Ranks::SIX:
            return "6";
        case Ranks::SEVEN:
           return "7";
        case Ranks::EIGHT:
           return "8";
        case Ranks::NINE:
            return "9";
        case Ranks::TEN:
            return "10";
        case Ranks::JACK:
           return "J"; // For face cards, we'll be returning the card itself and not the numeric value
        case Ranks::QUEEN:
            return "Q";
        case Ranks::KING:
           return "K";
        case Ranks::ACE:
            return "A";
    }
    return "";
}

bool Card::isAce() const{
    if(rank == Ranks::ACE){
        return true;    
    }
    return false;
}


bool Card::operator == (const Card &other) const{
    if ((suit == other.suit)&&(rank == other.rank)){
        return true;
    }
    return false;
}

int Card::getValue() const{
    if(rank >= Ranks::TWO && rank <= Ranks::TEN){
        return static_cast<int>(rank); // enum to int conversion
    }
    else if(rank == Ranks::ACE){ // what if score exceeds -> we deal hardAce
        return 11; // the value of Ace is set to 11(Can be changed to 1 later on)
    }
    else{
       return 10;
    }
}


