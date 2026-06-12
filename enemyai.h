#ifndef ENEMYAI_H
#define ENEMYAI_H


#include <vector>
#include "TemplateDeck.h"
class GameManager;

class EnemyAI
{

    // Pomocnicza lista dostępnych typów kart do wylosowania przez AI
    std::vector<CreatureType> availableCreatures_;

public:

public:
    void makeMove(GameManager* manager);
    int damageTaken;
    EnemyAI();


    // Główna metoda wywoływana przez silnik gry przed turą gracza
    bool executeDirectorTurn(GameManager* manager, sf::RenderWindow& window);

};

#endif // ENEMYAI_H
