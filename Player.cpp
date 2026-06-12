#include "Player.h"
#include "Card.h"
#include "context.h"
#include <algorithm>
#include <random>
#include <iostream>

void Player::addCard(const Card* c){
    //deck.emplace_back(cloneCard(c));
}


void Player::endBattleCleanup() {
    // 1. Usuwamy klony kart, które zostały w talii bitewnej i nie zostały dobrane
    for (Card* c : battleDeck) {
        if (c != nullptr) {
            delete c;
        }
    }
    battleDeck.clear();

    // 2. Usuwamy klony kart, które na koniec bitwy gracz trzymał w ręce
    for (Card* c : hand) {
        if (c != nullptr) {
            delete c;
        }
    }
    hand.clear();

    std::cout << "Bitwa zakonczona. Talia bitewna i reka zostaly bezpiecznie wyczyszczone.\n";
}


void Player::drawSquirrel(Card* squirrelCard) {
    if (squirrelCard == nullptr) return;

    // 1. Dodajemy w pełni zbudowaną kartę wiewiórki bezpośrednio do ręki
    hand.push_back(squirrelCard);

    // 2. Przeliczamy pozycje kart na dole ekranu, aby uwzględnić nowy element
    updateHandPositions();

    std::cout << "Player: Wiewiorka zostala dodana do reki i wysrodkowana.\n";
}


void Player::updateHandPositions() {
    if (hand.empty()) return;

    float screenWidth = 1920.f;
    float screenHeight = 1080.f;

    // Pobieramy bazowy rozmiar sprite'a karty (z jej tła) pomnożony przez aktualną skalę
    float cardWidth = hand[0]->background.getSize().x * hand[0]->getScale().x;
    float cardHeight = hand[0]->background.getSize().y * hand[0]->getScale().y;

    // Jeśli z jakiegoś powodu tekstura się nie załadowała, ustawiamy hardkodowany bezpiecznik (np. rozmiar standardowej karty)
    if (cardWidth == 0) cardWidth = 140.f;
    if (cardHeight == 0) cardHeight = 190.f;

    float spacing = 20.f; // Odstęp między kartami w ręce
    float handY = screenHeight - cardHeight - 50.f; // 50px nad dolną krawędzią ekranu

    // Obliczamy całkowitą szerokość całej ręki, aby ją idealnie wycentrować
    float totalWidth = (hand.size() * cardWidth) + ((hand.size() - 1) * spacing);
    float startX = (screenWidth - totalWidth) / 2.f;

    for (size_t i = 0; i < hand.size(); ++i) {
        float finalX = startX + i * (cardWidth + spacing);
        hand[i]->setPosition(finalX, handY);
    }
}


///loads some basic ass cards into permanent deck
void Player::initializeDeck(Context* context, sf::RenderWindow& window){
    Card* card = context->manager_->BuildCard(2,5,"Raven",2,CostType::BLOOD,context->textures_["raven"],context->textures_["card"],context->fonts_["papyrus"],window,0);
    //manager_->placeCard(card,0,0);

    Card* card3 = context->manager_->BuildCard(3,2,"Wolf",2,CostType::BLOOD,context->textures_["wolf"],context->textures_["card"],context->fonts_["papyrus"],window,0);
    //manager_->placeCard(card3,1,0);

    Card* card1 = context->manager_->BuildCard(1,3,"Snake",1,CostType::BLOOD,context->textures_["snake"],context->textures_["card"],context->fonts_["papyrus"],window,0);
    //manager_->placeCard(card1,1,2);

    Card* card2 = context->manager_->BuildCard(1,3,"Snake",1,CostType::BLOOD,context->textures_["snake"],context->textures_["card"],context->fonts_["papyrus"],window,0);
    //manager_->placeCard(card2,0,3);

    permanentDeck.emplace_back(card);
    permanentDeck.emplace_back(card1);
    permanentDeck.emplace_back(card2);
    permanentDeck.emplace_back(card3);

}



void Player::prepareForBattle(GameManager* manager) {
    // 1. Czyszczenie starych pozostałości po poprzedniej bitwie
    for (Card* c : battleDeck) delete c;
    for (Card* c : hand) delete c;

    battleDeck.clear();
    hand.clear();

    if (manager == nullptr) return;

    // 1. Czyszczenie starych pozostałości
    for (Card* c : battleDeck) delete c;
    for (Card* c : hand) delete c;

    battleDeck.clear();
    hand.clear();

    // 2. Klonowanie kart z talii stałej przy użyciu managera
    for (const Card* baseCard : permanentDeck) {
        if (baseCard != nullptr) {
            // TUTAJ: manager->cloneCard poprawnie wywoła BuildCard dla każdej karty!
            Card* battleCopy = manager->cloneCard(baseCard);
            battleDeck.push_back(battleCopy);
        }
    }

    // 3. Tasowanie talii bitewnej
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(battleDeck.begin(), battleDeck.end(), g);

    // 4. Dobranie początkowej ręki (3 karty na start)
    for (int i = 0; i < 3; ++i) {
        drawCard();
    }
}

bool Player::drawCard() {
    // Bezpiecznik: jeśli talia jest pusta, gracz nie może dobrać karty
    if (battleDeck.empty()) {
        //std::cout << "Brak kart w talii bitewnej!" << std::endl;
        return false;
    }

    // Pobieramy wskaźnik na ostatnią kartę z talii
    Card* drawnCard = battleDeck.back();

    // Usuwamy wskaźnik z talii (obiekt w pamięci RAM cały czas istnieje!)
    battleDeck.pop_back();

    // Wrzucamy kartę do ręki gracza
    hand.push_back(drawnCard);

    // Aktualizujemy pozycje X i Y kart w ręce, aby ładnie rozłożyły się na pasku
    updateHandPositions();

    return true;
}

void Player::removeCard(Card* c) {
    auto it = std::find(deck.begin(), deck.end(), c);
    if (it != deck.end()) {
        delete *it;
        deck.erase(it);
    }
}



