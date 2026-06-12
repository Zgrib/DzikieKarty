#include "BattleEngine.h"
#include "GameManager.h"
#include "GameLog.h"
#include <iostream>
void BattleEngine::setGM(GameManager* manager){
    manager_ = manager;
}

void BattleEngine::setCardInSlot(Card* card, int row, int col) {
    if (row >= 0 && row < 2 && col >= 0 && col < 4) {
        board[col][row] = card;
    }
}
void BattleEngine::EndTurn() {
    currentProcessCol = 0;
    currentProcessRow = 1;

    timer.restart();
    // Od razu odpalamy proces walki!
    currentState = BattleState::CARD_PROCESSING;


}

void BattleEngine::goToNextRoundDirectorPlanning() {


    // Dodajemy zasoby dla Directora na poczet nowej rundy
    if (manager_ != nullptr) {
        manager_->aiDirectorCredits += 3;
        GameLog::add("-> AI planuje kolejne ruchy...");
    }

    timer.restart();
    // Przechodzimy do fazy, w której AI wystawi nowe potwory przed ruchem gracza
    currentState = BattleState::DIRECTOR_PHASE;
}

bool BattleEngine::isSlotEmpty(int row, int col) {
    if (row >= 0 && row < 2 && col >= 0 && col < 4) {
        return board[col][row] == nullptr; // <-- TUTAJ [col][row]
    }
    return false;
}

void BattleEngine::moveToNextCard() {
    currentProcessCol++;

    if (currentProcessCol >= 4) {
        if (abs(player->damageTaken - eai->damageTaken) >= 5) {
            if (player->damageTaken > eai->damageTaken) GameLog::add("PRZEGRANA! Koniec bitwy.");
            else GameLog::add("WYGRANA! Koniec bitwy.");
            EndBattle();
            manager_->canDraw = false;
            currentState = BattleState::BATTLE_OVER;
            return;
        }

        currentProcessCol = 0;
        currentProcessRow--;
    }

    if (currentProcessRow < 0) {
        // Po skończonej walce przechodzimy do planowania Directora
        goToNextRoundDirectorPlanning();
    }
    else {
        currentState = BattleState::CARD_PROCESSING;
    }
}


void BattleEngine::update(float deltaTime) {
    switch (currentState) {
    case BattleState::IDLE:
        break;

    case BattleState::START_PHASE:
        //std::cout << "Rozpoczynam bitwe. Faza wstepna Directora...\n";

        manager_->aiDirectorCredits += 3;
        timer.restart();
        currentState = BattleState::DIRECTOR_PHASE;
        break;

    case BattleState::CARD_PROCESSING: {
        if (manager_ != nullptr) {
            manager_->cleanupDeadCards();
        }

        if (currentProcessCol < 0 || currentProcessCol >= 4 || currentProcessRow < 0 || currentProcessRow >= 2) {
            if (currentProcessRow < 0) {
                goToNextRoundDirectorPlanning();
            } else {
                currentProcessCol = 0;
                currentProcessRow--;
                if (currentProcessRow < 0) {
                    goToNextRoundDirectorPlanning();
                }
            }
            return;
        }

        Card* activeCard = board[currentProcessCol][currentProcessRow];

        if (activeCard != nullptr && activeCard->getHealth() > 0 && activeCard->getDamage() > 0 ) {

            int opponentRow = (currentProcessRow == 1) ? 0 : 1;
            Card* opponentCard = board[currentProcessCol][opponentRow];

            std::cout << "Karta na pozycji [col: " << currentProcessCol << "][row: " << currentProcessRow
                      << "] atakuje wroga naprzeciwko [col: " << currentProcessCol << "][row: " << opponentRow << "]\n";

            if (opponentCard != nullptr) {
                opponentCard->health -= activeCard->getDamage();
                GameLog::add("-> " + activeCard->name + " atakuje " + opponentCard->name + " zadajac " + std::to_string(activeCard->getDamage()) + " obrazen.");

            } else {
                if (opponentRow == 0) {
                    eai->damageTaken += activeCard->getDamage();
                    GameLog::add("-> " + activeCard->name + " atakuje wroga bezposrednio zadajac " + std::to_string(activeCard->getDamage()) + " obrazen.");
                } else {
                    player->damageTaken += activeCard->getDamage();
                    GameLog::add("-> " + activeCard->name + " atakuje ciebie bezposrednio zadajac " + std::to_string(activeCard->getDamage()) + " obrazen.");
                }
            }

            timer.restart();
            currentState = BattleState::COOLDOWN;
        }
        else {
            // Jeśli slot pusty lub karta uśpiona/martwa – przesuń na następną
            currentProcessCol++;
            if (currentProcessCol >= 4) {
                if (abs(player->damageTaken - eai->damageTaken) >= 5) {
                    if (player->damageTaken > eai->damageTaken){
                        manager_->hasLost=true;
                        GameLog::add("PRZEGRANA! Koniec bitwy.");
                    }
                    else GameLog::add("WYGRANA! Koniec bitwy.");
                    EndBattle();
                    manager_->canDraw = false;
                    currentState = BattleState::BATTLE_OVER;
                    return;
                }
                currentProcessCol = 0;
                currentProcessRow--;
            }

            if (currentProcessRow < 0) {
                goToNextRoundDirectorPlanning();
            } else {
                currentState = BattleState::CARD_PROCESSING;
            }
            return;
        }
        break;
    }

    case BattleState::COOLDOWN:
        if (timer.getElapsedTime().asSeconds() >= delayBetweenActions) {
            currentProcessCol++;
            manager_->cleanupDeadCards();

            if (currentProcessCol >= 4) {
                if (abs(player->damageTaken - eai->damageTaken) >= 5) {
                    if (player->damageTaken > eai->damageTaken){
                        manager_->hasLost=true;
                        GameLog::add("PRZEGRANA! Koniec bitwy.");
                    }
                    else GameLog::add("WYGRANA! Koniec bitwy.");
                    EndBattle();
                    manager_->canDraw = false;
                    currentState = BattleState::BATTLE_OVER;
                    return;
                }
                currentProcessCol = 0;
                currentProcessRow--;
            }

            if (currentProcessRow < 0) {
                // PUNKT 7: Koniec wszystkich ataków -> Przejście do Directora
                goToNextRoundDirectorPlanning();
            } else {
                currentState = BattleState::CARD_PROCESSING;
            }
        }
        break;

    case BattleState::DIRECTOR_PHASE:
        if (timer.getElapsedTime().asSeconds() >= delayBetweenActions) {
            sf::RenderWindow& window = (manager_->getWindow());

            EnemyAI& director = manager_->getAI();
            bool cardSpawned = director.executeDirectorTurn(manager_, window);

            if (cardSpawned) {
                timer.restart(); // Director położył kartę, czekamy na kolejny krok
            } else {
                // Director skończył stawiać karty. DOPIERO TERAZ gracz dostaje ruch!
                currentState = BattleState::END_PHASE;
            }
        }
        break;

    case BattleState::END_PHASE:
        GameLog::add("Twoja tura! Dobierz karte.");
        manager_->canDraw = true;

        // Czyszczenie i usuwanie martwych kart na koniec fazy
        if (manager_ != nullptr) {
            manager_->cleanupDeadCards();
        }

        // Oddajemy pełną kontrolę graczowi (PUNKT 3)
        currentState = BattleState::IDLE;
        break;

    case BattleState::BATTLE_OVER:
        break;
    }
}



void BattleEngine::EndBattle(){
    player->endBattleCleanup();
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 2; ++r) {
            //delete manager_->getBattleEngine().board[c][r];
            manager_->getBattleEngine().board[c][r] = nullptr;
        }
    }

}