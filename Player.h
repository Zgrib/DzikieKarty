#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "Card.h"


class Player{
private:
    std::vector<Card*> deck;
    std::vector<Card*> permanentDeck;

public:
    std::vector<Card*> battleDeck;
    std::vector<Card*> hand;



    Player(){}
    void prepareForBattle();
    void drawCard();
    void updateHandPositions();


    ~Player() {
        for (auto* c : deck) delete c;
        deck.clear();
    }
    int damageTaken=0;


    void addCard(const Card* c);


    std::vector<Card> cloneDeck();

    void removeCard(Card* c);


};












#endif // PLAYER_H
