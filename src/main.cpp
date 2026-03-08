#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Game.h"
#include <iostream>
#include <vector>
#include <string>

int main(){
    // Option 1: Run Flask server (when you integrate Flask)
    // system("python flask_app.py");
    
    // Option 2: CLI testing (current approach - keep for debugging)
    Game g = Game();
    std::cout << "Welcome to Blackjack (Terminal)\n";
    std::cout << "Press 1 to test a round \n";
    std::cout << "Press 2 to exit \n";
    
    int choice;
    std::cin >> choice;
    
    if(choice == 1){
        g.play();
        g.closeRound();
    }
    
    return 0;
}