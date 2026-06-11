#include <vector>
#include "Card.h"
#include "Player.h"
#include "enemyai.h"

#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H
enum class BattleState {
    IDLE,            // Oczekiwanie na ruch gracza
    START_PHASE,     // Przygotowanie do walki
    CARD_PROCESSING, // Aktywacja konkretnej karty (zadawanie obrażeń)
    COOLDOWN,        // Krótka pauza (np. 0.5 sekundy), aby gracz zauważył co się stało
    END_PHASE        // Sprzątanie po turze, oddanie ruchu
};
class GameManager;
class BattleEngine{
private:
    //może lepiej aby to były sloty na kartę a nie po prostu karty???
    //to chyba zależy, tak też można
    //std::vector<std::vector<Card*>> board;
    Card* board[4][2];




public:



    BattleState currentState = BattleState::IDLE;

    // Surowa tablica kart z Twojego kodu (zakładam 4 kolumny, 2 rzędy)

    // Zmienne do śledzenia kroku symulacji
    int currentProcessRow = 0;
    int currentProcessCol = 0;

    sf::Clock timer;
    float delayBetweenActions = 0.6f; // Czas trwania "kroku" w sekundach (np. 600 ms)
    void update(float deltaTime);
    void setCardInSlot(Card* card, int row, int col);
    Player* player;
    EnemyAI* eai;
    GameManager* manager_=nullptr;

    void setGM(GameManager* manager);


    BattleEngine(){

        for(auto& v:board){
            for(auto& ptr:v){
                ptr=nullptr;
            }
        }

        player = new Player();
        eai = new EnemyAI();
    }

    bool isSlotEmpty(int row, int col);
    ///unsure if this will be neccessary??
    void PrepareBattle(){

        for(auto& v:board){
            for(auto& ptr:v){
                ptr=nullptr;
            }
        }


    }

    void EndTurn();

    void EndBattle();

};

#endif // BATTLEENGINE_H
