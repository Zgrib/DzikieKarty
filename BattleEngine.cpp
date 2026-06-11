#include "BattleEngine.h"
#include "GameManager.h"
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
        // POPRAWKA: Indeksowanie [col][row] zgodnie z Twoją deklaracją board[4][2]
        Card* activeCard = board[currentProcessCol][currentProcessRow];

        if (activeCard != nullptr && activeCard->getDamage() > 0) {

            // Wyznaczamy rząd przeciwnika (nadal operujemy na wartościach 0 i 1)
            int opponentRow = (currentProcessRow == 1) ? 0 : 1;

            // POPRAWKA: Tutaj również zmieniamy na [col][row]
            Card* opponentCard = board[currentProcessCol][opponentRow];

            std::cout << "Karta na pozycji [col: " << currentProcessCol << "][row: " << currentProcessRow
                      << "] atakuje wroga naprzeciwko [col: " << currentProcessCol << "][row: " << opponentRow << "]\n";

            if (opponentCard != nullptr) {
                opponentCard->health -= activeCard->getDamage();
                std::cout << "Zadano " << activeCard->getDamage() << " dmg. HP wroga: " << opponentCard->health << "\n";
            } else {
                std::cout << "Pozycja naprzeciwko jest pusta.\n";
            }

            timer.restart();
            currentState = BattleState::COOLDOWN;
        }
        else {
            // Pusty slot lub 0 ataku - natychmiastowy przeskok w tej samej klatce
            currentProcessCol++;
            if (currentProcessCol >= 4) {
                currentProcessCol = 0;
                currentProcessRow--; // Ruch w górę (z row 1 na row 0)
            }

            if (currentProcessRow < 0) {
                currentState = BattleState::END_PHASE;
            } else {
                currentState = BattleState::CARD_PROCESSING;
            }
        }
        break;

    }

    case BattleState::COOLDOWN:
        // Czekamy, aż upłynie zdefiniowany czas delayBetweenActions
        if (timer.getElapsedTime().asSeconds() >= delayBetweenActions) {

            // Przechodzimy do kolejnej kolumny w prawo (0 -> 1 -> 2 -> 3)
            currentProcessCol++;

            if (currentProcessCol >= 4) {
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
        std::cout << "Koniec symulacji. Wszystkie karty wykonaly ruch.\n";

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
    }
}