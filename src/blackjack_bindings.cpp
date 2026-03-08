#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Game.h"

namespace py = pybind11;

PYBIND11_MODULE(blackjack_game, m) {
    m.doc() = "Blackjack game C++ bindings";

    // Enums
    py::enum_<Suits>(m, "Suits")
        .value("HEARTS", Suits::HEARTS)
        .value("DIAMONDS", Suits::DIAMONDS)
        .value("CLUBS", Suits::CLUBS)
        .value("SPADES", Suits::SPADES)
        .export_values();

    py::enum_<Ranks>(m, "Ranks")
        .value("TWO", Ranks::TWO)
        .value("THREE", Ranks::THREE)
        .value("FOUR", Ranks::FOUR)
        .value("FIVE", Ranks::FIVE)
        .value("SIX", Ranks::SIX)
        .value("SEVEN", Ranks::SEVEN)
        .value("EIGHT", Ranks::EIGHT)
        .value("NINE", Ranks::NINE)
        .value("TEN", Ranks::TEN)
        .value("JACK", Ranks::JACK)
        .value("QUEEN", Ranks::QUEEN)
        .value("KING", Ranks::KING)
        .value("ACE", Ranks::ACE)
        .export_values();

    // Card class
    py::class_<Card>(m, "Card")
        .def(py::init<Suits, Ranks>())
        .def_readwrite("suit", &Card::suit)
        .def_readwrite("rank", &Card::rank)
        .def("getSuit", &Card::getSuit)
        .def("getRank", &Card::getRank)
        .def("getSuitString", &Card::getSuitString)
        .def("getRankString", &Card::getRankString)
        .def("getValue", &Card::getValue)
        .def("isAce", &Card::isAce);

    // Deck class
    py::class_<Deck>(m, "Deck")
        .def(py::init<>())
        .def("isEmpty", &Deck::isEmpty)
        .def("RandomShuffle", &Deck::RandomShuffle)
        .def("reset", &Deck::reset)
        .def("GiveCard", &Deck::GiveCard);

    // Player class
    py::class_<Player>(m, "Player")
        .def(py::init<>())
        .def_readwrite("hand", &Player::hand)
        .def_readwrite("Bankbalance", &Player::Bankbalance)
        .def_readwrite("betamt", &Player::betamt)
        .def_readwrite("isStanding", &Player::isStanding)
        .def_readwrite("hasSurrendered", &Player::hasSurrendered)
        .def_readwrite("diddoubledown", &Player::diddoubledown)
        .def_readwrite("didsplit", &Player::didsplit)
        .def_readwrite("Splitbet", &Player::Splitbet)
        .def_readwrite("Splithand", &Player::Splithand)
        .def_readwrite("Insured", &Player::Insured)
        .def_readwrite("busted", &Player::busted)
        .def_readwrite("pBJstatus", &Player::pBJstatus)
        .def_readwrite("splithandbust", &Player::splithandbust)
        .def("hasbetamt", &Player::hasbetamt)
        .def("Placebet", &Player::Placebet)
        .def("initialhand", &Player::initialhand)
        .def("handvalue", &Player::handvalue)
        .def("totalbet", &Player::totalbet)
        .def("getBetamt", &Player::getBetamt)
        .def("hit", &Player::hit)
        .def("stand", &Player::stand)
        .def("candoubledown", &Player::candoubledown)
        .def("doubledown", &Player::doubledown)
        .def("cansplit", &Player::cansplit)
        .def("split", &Player::split)
        .def("addchips", &Player::addchips)
        .def("removechips", &Player::removechips)
        .def("bust", &Player::bust)
        .def("hasBlackjackf", &Player::hasBlackjackf)
        .def("isStandingf", &Player::isStandingf)
        .def("Clearhand", &Player::Clearhand);

    // Dealer class
    py::class_<Dealer>(m, "Dealer")
        .def(py::init<>())
        .def_readwrite("Dealerhand", &Dealer::Dealerhand)
        .def_readwrite("holeCard", &Dealer::holeCard)
        .def_readwrite("dBJstatus", &Dealer::dBJstatus)
        .def("getinitialhand", &Dealer::getinitialhand)
        .def("Dealerhandvalue", &Dealer::Dealerhandvalue)
        .def("DealerBlackjack", &Dealer::DealerBlackjack)
        .def("hit", &Dealer::hit)
        .def("clearhand", &Dealer::clearhand)
        .def("Dealerbust", &Dealer::Dealerbust);

    // Game class
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def_readwrite("p", &Game::p)
        .def_readwrite("d", &Game::d)
        .def_readwrite("deck", &Game::deck)
        .def_readwrite("handpick", &Game::handpick)
        .def("Betting", &Game::Betting)
        .def("Initialhands", &Game::Initialhands)
        .def("Playeroptions", &Game::Playeroptions)
        .def("DealerChance", &Game::DealerChance)
        .def("settleHand", &Game::settleHand)
        .def("Result", &Game::Result)
        .def("play", &Game::play);
}

