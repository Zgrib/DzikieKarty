#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "BattleEngine.h"
#include "GameBoard.h"
//#include "Card.cpp"

class GameManager {
private:
    sf::RenderWindow& window_;
    BattleEngine battleEngine_;
    GameBoard* board_;
    // Lista kart aktualnie znajdujących się na planszy do renderowania
    std::vector<Card*> deployedCards_;

    struct VisualSlot {
        sf::Sprite sprite;
        int row;
        int col;
    };
    std::vector<VisualSlot> visualSlots_;
    sf::Texture slotTexture_;

    Player* player;
    EnemyAI* eai;


public:
    bool canDraw=false;
    CustomTextDrawable* cardCounter=nullptr;
    Card* selectedCard=nullptr;
    std::vector<Card*> cardsToSacrifice;


    GameManager(sf::RenderWindow& window);
    ~GameManager();

    // Metoda odpowiedzialna za logiczne i graficzne postawienie karty
    bool placeCard(Card* card, int row, int col);

    // Wywoływane wewnątrz pętli rysowania w Context::battleground
    void drawBoardElements();

    Player& getPlayer();

    EnemyAI& getAI();

    void initVisualSlots(const sf::Texture& tex);

    bool handleBoardClick(sf::Vector2f mousePos);

    void cleanupDeadCards();

    Card* BuildCard(int _damage, int _health, std::string _name, int _cost, CostType _ct,
                    const sf::Texture& _texture, const sf::Texture& _background,
                    const sf::Font& _font, sf::RenderWindow &window, int z);

    Card* cloneCard(const Card* c);

    BattleEngine& getBattleEngine();

    void removeDeployedCard(Card* card);

    // Gettery
    GameBoard* getBoard() const;

    bool tryPlayCard(Player& player, Card* card, int targetRow, int targetCol);
};

#endif // GAMEMANAGER_H