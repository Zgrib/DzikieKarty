#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "BattleEngine.h"
#include "GameBoard.h"
#include "TemplateDeck.h"
//#include "Card.cpp"
class Context;
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
    Context* context;

public:
    int aiDirectorBudgetIncrease=0;

    int aiDirectorBudget;

    int aiDirectorCredits;
    void resetBattleground();
    std::map<CreatureType, int> aiCardValues = {
        { CreatureType::WEZ,        4 },
        { CreatureType::NIETOPERZ,  3 },
        { CreatureType::WILK,       5 },
        { CreatureType::WRONA,      6 },
        { CreatureType::KARALUCH,   2},
        { CreatureType::OPOS,   1}
    };

    sf::Font& getFont(std::string fontname);

    sf::Texture& getTextureCreature(CreatureType type);
    sf::Texture& getTexture(std::string tex);


    bool bloodCostPaid = false;
    int bones=0;
    bool canDraw=false;
    CustomTextDrawable* cardCounter=nullptr;
    Card* selectedCard=nullptr;
    std::vector<Card*> cardsToSacrifice;

    sf::RenderWindow& getWindow();
    GameManager(sf::RenderWindow& window, Context* c);
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

    Card* BuildCard(const CardStats& stats, const sf::Texture& _texture, const sf::Texture& _background, const sf::Font& _font, sf::RenderWindow &window, int z);
    /*
    Card* BuildCard(int _damage, int _health, std::string _name, int _cost, CostType _ct,
                    const sf::Texture& _texture, const sf::Texture& _background,
                    const sf::Font& _font, sf::RenderWindow &window, int z);
    */

    Card* cloneCard(const Card* c);

    BattleEngine& getBattleEngine();

    void removeDeployedCard(Card* card);

    // Gettery
    GameBoard* getBoard() const;

    bool tryPlayCard(Player& player, Card* card, int targetRow, int targetCol);
};

#endif // GAMEMANAGER_H