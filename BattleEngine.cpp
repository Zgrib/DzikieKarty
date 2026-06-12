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
    if (currentState == BattleState::IDLE) {

        currentState = BattleState::START_PHASE;
        currentProcessRow = 1;
        currentProcessCol = 0;
        timer.restart();
    }
}

bool BattleEngine::isSlotEmpty(int row, int col) {
    if (row >= 0 && row < 2 && col >= 0 && col < 4) {
        return board[col][row] == nullptr; // <-- TUTAJ [col][row]
    }
    return false;
}

void BattleEngine::update(float deltaTime) {
    switch (currentState) {
    case BattleState::IDLE:
        break;

    case BattleState::START_PHASE:
        std::cout << "Rozpoczynam symulacje konca tury...\n";
        currentState = BattleState::CARD_PROCESSING;
        break;

    case BattleState::CARD_PROCESSING: {
        if (manager_ != nullptr) {
            manager_->cleanupDeadCards();
        }

        if (currentProcessCol < 0 || currentProcessCol >= 4 || currentProcessRow < 0 || currentProcessRow >= 2) {
            currentState = BattleState::END_PHASE;
            return;
        }

        // POPRAWKA: Indeksowanie [col][row] zgodnie z Twoją deklaracją board[4][2]
        Card* activeCard = board[currentProcessCol][currentProcessRow];

        if (activeCard != nullptr && activeCard->getHealth() > 0 && activeCard->getDamage() > 0) {

            // Wyznaczamy rząd przeciwnika (nadal operujemy na wartościach 0 i 1)
            int opponentRow = (currentProcessRow == 1) ? 0 : 1;

            // POPRAWKA: Tutaj również zmieniamy na [col][row]
            Card* opponentCard = board[currentProcessCol][opponentRow];

            std::cout << "Karta na pozycji [col: " << currentProcessCol << "][row: " << currentProcessRow
                      << "] atakuje wroga naprzeciwko [col: " << currentProcessCol << "][row: " << opponentRow << "]\n";

            if (opponentCard != nullptr) {
                opponentCard->health -= activeCard->getDamage();
                GameLog::add("-> "+activeCard->name +" atakuje " +opponentCard->name + " zadajac " + std::to_string(activeCard->getDamage()) +" obrazen.");
                std::cout << "Zadano " << activeCard->getDamage() << " dmg. HP wroga: " << opponentCard->health << "\n";
            } else {
                if(opponentRow==0){
                    eai->damageTaken += activeCard->getDamage();
                    GameLog::add("-> "+activeCard->name +" atakuje wroga bezposrednio zadajac " +  std::to_string(activeCard->getDamage()) +" obrazen.");
                }
                else{
                    player->damageTaken += activeCard->getDamage();
                    GameLog::add("-> "+activeCard->name +" atakuje ciebie bezposrednio zadajac " +  std::to_string(activeCard->getDamage()) +" obrazen.");
                }

                //std::cout << "Pozycja naprzeciwko jest pusta.\n";
            }

            timer.restart();
            currentState = BattleState::COOLDOWN;
        }
        else {
            currentProcessCol++;
            if (currentProcessCol >= 4) {
                //logika (potencjalnego) konca bitwy

                if(abs(player->damageTaken - eai->damageTaken)>=5){
                    //battle has to end
                    if(player->damageTaken > eai->damageTaken){
                        GameLog::add("PRZEGRANA! Koniec bitwy.");
                        //player loses
                        //TUTAJ powinna konczyc sie bitwa!!
                    }
                    else{
                        GameLog::add("WYGRANA! Koniec bitwy.");
                        //player wins
                        //TUTAJ powinna konczyc sie bitwa!!
                    }
                    EndBattle();
                    manager_->canDraw = false;
                    currentState = BattleState::BATTLE_OVER; // <--- Aktywacja stanu końca
                    return;

                }

                currentProcessCol = 0;
                currentProcessRow--; // Ruch w górę (z row 1 na row 0)
            }

            if (currentProcessRow < 0) {
                currentState = BattleState::END_PHASE;
            } else {
                currentState = BattleState::CARD_PROCESSING;
            }

            return;
        }


        break;

    }

    case BattleState::COOLDOWN:
        // Czekamy, aż upłynie zdefiniowany czas delayBetweenActions
        if (timer.getElapsedTime().asSeconds() >= delayBetweenActions) {

            // Przechodzimy do kolejnej kolumny w prawo (0 -> 1 -> 2 -> 3)
            currentProcessCol++;

            if (currentProcessCol >= 4) {

                // === DRUGIE MIEJSCE: SPRAWDZENIE KOŃCA BITWY (Gdy karta zaatakowała i cooldown minął) ===
                if (abs(player->damageTaken - eai->damageTaken) >= 5) {
                    if (player->damageTaken > eai->damageTaken) {
                        GameLog::add("PRZEGRANA! Koniec bitwy.");
                    } else {
                        GameLog::add("WYGRANA! Koniec bitwy.");
                    }
                    EndBattle();
                    manager_->canDraw = false;
                    currentState = BattleState::BATTLE_OVER; // <--- Aktywacja stanu końca
                    return;
                }

                currentProcessCol = 0; // Resetujemy kolumnę do lewej krawędzi
                currentProcessRow--;   // <--- IDZIEMY W GÓRĘ (z row=1 przechodzimy na row=0)
            }

            // Bezpiecznik końca planszy:
            // Skoro zaczęliśmy na row=1, a potem zmniejszyliśmy do row=0,
            // to koniec nastąpi, gdy row spadnie poniżej zera (< 0)
            if (currentProcessRow < 0) {
                currentState = BattleState::END_PHASE;
            } else {
                // Na planszy są jeszcze rzędy do przetworzenia (czyli rząd 0)
                currentState = BattleState::CARD_PROCESSING;
            }
        }
        break;

    case BattleState::END_PHASE:
        GameLog::add("Twoja tura, dobierz karte!");
        manager_->canDraw=true;
        // Usunięcie martwych kart z tablicy silnika (HP <= 0)
        for(int r = 0; r < 2; ++r) {
            for(int c = 0; c < 4; ++c) {
                if(board[c][r] != nullptr && board[c][r]->health <= 0) { // <-- TUTAJ [c][r]
                    board[c][r] = nullptr;

                }
            }
        }
        if (manager_ != nullptr) {
            manager_->cleanupDeadCards();
        }
        currentState = BattleState::IDLE; // Powrót do oczekiwania na gracza
        break;


    case BattleState::BATTLE_OVER:
        break;
    }
}

void BattleEngine::EndBattle(){
    player->endBattleCleanup();
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 2; ++r) {
            manager_->getBattleEngine().board[c][r] = nullptr;
        }
    }

}