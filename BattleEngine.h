#include <vector>
#include "Card.h"
#include "Player.h"
#include "enemyai.h"

#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H
enum class BattleState {
    IDLE,
    START_PHASE,
    CARD_PROCESSING,
    COOLDOWN,
    DIRECTOR_PHASE,
    END_PHASE,
    BATTLE_OVER
};
class GameManager;
class BattleEngine{
private:
    //może lepiej aby to były sloty na kartę a nie po prostu karty???
    //to chyba zależy, tak też można
    //std::vector<std::vector<Card*>> board;




public:
    Card* board[4][2];



    BattleState currentState = BattleState::IDLE;
    BattleState getCurrentState(){
        return currentState;
    }
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
    //void goToDirectorPhase();
    void moveToNextCard();
    void goToNextRoundDirectorPlanning();
    BattleEngine(){
        currentState = BattleState::IDLE;
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
        player->prepareForBattle(manager_);
        //eai prepare
    }

    void EndTurn();

    void EndBattle();

};

#endif // BATTLEENGINE_H
