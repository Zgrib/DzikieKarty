#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "Card.h"
class GameManager;
class Context;

class Player{
private:

    std::vector<Card*> permanentDeck;

public:
    std::vector<Card*> battleDeck;
    std::vector<Card*> hand;



    Player(){damageTaken=0;}
    void prepareForBattle(GameManager* manager);
    bool drawCard();
    void updateHandPositions();
    void initializeDeck(Context* manager, sf::RenderWindow& window);
    void endBattleCleanup();

    void drawSquirrel(Card* squirrelCard);

    ~Player() {
        for (Card* c : permanentDeck) {
            if (c != nullptr) {
                delete c;
            }
        }
        permanentDeck.clear();
    }
    int damageTaken=0;


    void addCard(const Card* c);


    std::vector<Card> cloneDeck();

    void removeCard(Card* c);


};












#endif // PLAYER_H
