#include "enemyai.h"
#include "EnemyAI.h"
#include "GameManager.h"
#include "GameLog.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

EnemyAI::EnemyAI() {
    damageTaken=0;
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    // Lista potworów, które AI bierze pod uwagę przy losowaniu
    availableCreatures_ = {
        CreatureType::WEZ,
        CreatureType::NIETOPERZ,
        CreatureType::WILK,
        CreatureType::WRONA,
        CreatureType::KARALUCH
    };

}

void EnemyAI::makeMove(GameManager* manager) {
    if (manager == nullptr) return;

    // Pobieramy referencję do okna, która jest przechowywana w GameManagerze
    // (Zakładam, że Twój GameManager ma metodę getWindow() lub publiczne pole window)
    sf::RenderWindow& window = manager->getWindow();

    std::cout << "Director AI rozpoczyna zagrywanie kart. Dostepne kredyty: "
              << manager->aiDirectorCredits << "\n";

    bool cardPlayed = true;
    int cardsPlayedCount = 0;

    // Pętla wykonuje się tak długo, jak executeDirectorTurn odnosi sukces (czyli stać go i ma miejsce)
    while (cardPlayed) {
        cardPlayed = executeDirectorTurn(manager, window);
        if (cardPlayed) {
            cardsPlayedCount++;
        }
    }

    std::cout << "Director AI zakonczyl ruch. Zagrano kart: " << cardsPlayedCount
              << ", pozostale kredyty: " << manager->aiDirectorCredits << "\n";
}

bool EnemyAI::executeDirectorTurn(GameManager* manager, sf::RenderWindow& window) {
    if (manager->aiDirectorCredits <= 0) return false;

    // 1. Director sprawdza, które pola są wolne
    std::vector<int> freeColumns;
    for (int col = 0; col < 4; ++col) {
        // Sprawdzamy board[kolumna][rząd_0]
        if (manager->getBattleEngine().board[col][0] == nullptr) {
            freeColumns.push_back(col);
        }
    }

    // 2. Director sprawdza, czy istnieje JAKIEKOLWIEK wolne pole
    if (freeColumns.empty()) return false;

    TemplateDeck tmp_deck;

    for (int attempt = 0; attempt < 5; ++attempt) {
        int randomIdx = rand() % availableCreatures_.size();
        CreatureType selectedType = availableCreatures_[randomIdx];
        int cardCost = manager->aiCardValues[selectedType];

        if(manager->aiDirectorBudget <=0 )
            return false;

        if (manager->aiDirectorCredits >= cardCost) {
            int colIdx = rand() % freeColumns.size();
            int targetCol = freeColumns[colIdx];

            CardStats stats = tmp_deck.generateCardStats(selectedType);
            sf::Texture& creatureTex = manager->getTextureCreature(selectedType);
            sf::Texture& cardBackTex = manager->getTexture("card");

            Card* aiCard = manager->BuildCard(stats, creatureTex, cardBackTex, manager->getFont("papyrus"), window, 1);

            // === TUTAJ BYŁ BŁĄD - POPRAWIONE WYWOŁANIE ===
            // GameManager przyjmuje: placeCard(karta, rząd, kolumna)
            manager->placeCard(aiCard, 0, targetCol);



            manager->aiDirectorCredits -= cardCost;
            manager->aiDirectorBudget -=cardCost;
            GameLog::add("AI zagrywa: " + aiCard->name);

            return true;
        }
    }

    return false;
}