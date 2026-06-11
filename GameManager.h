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

    Card* selectedCard_=nullptr;
    std::vector<Card*> cardsToSacrifice_;


public:

    GameManager(sf::RenderWindow& window);
    ~GameManager();

    // Metoda odpowiedzialna za logiczne i graficzne postawienie karty
    bool placeCard(Card* card, int row, int col);

    // Wywoływane wewnątrz pętli rysowania w Context::battleground
    void drawBoardElements();

    void cleanupDeadCards();

    Card* BuildCard(int _damage, int _health, int _cost, CostType _ct,
                    const sf::Texture& _texture, const sf::Texture& _background,
                    const sf::Font& _font, sf::RenderWindow &window, int z);

    Card* cloneCard(const Card* c);

    BattleEngine& getBattleEngine();

    // Gettery
    GameBoard* getBoard() const;
};

#endif // GAMEMANAGER_H