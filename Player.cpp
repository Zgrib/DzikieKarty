#include "Player.h"
#include "Card.h"
#include "context.h"
#include "TemplateDeck.h"
#include <algorithm>
#include <random>
#include <iostream>

void Player::addCard(const Card* c){

    permanentDeck.emplace_back(c->clone());
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
void Player::initializeDeck(Context* context, sf::RenderWindow& window) {
    // Tworzymy lokalną fabrykę statystyk
    TemplateDeck tmp_deck;

    // Kruk
    CardStats ravenStats = tmp_deck.generateCardStats(CreatureType::WRONA);
    Card* card = context->manager_->BuildCard(ravenStats, context->textures_[ravenStats.textureKey], context->textures_["card"], context->fonts_["papyrus"], window, 0);

    // Wilk
    CardStats wolfStats = tmp_deck.generateCardStats(CreatureType::WILK);
    Card* card3 = context->manager_->BuildCard(wolfStats, context->textures_[wolfStats.textureKey], context->textures_["card"], context->fonts_["papyrus"], window, 0);

    // Waz 1
    CardStats snakeStats1 = tmp_deck.generateCardStats(CreatureType::WEZ);
    Card* card1 = context->manager_->BuildCard(snakeStats1, context->textures_[snakeStats1.textureKey], context->textures_["card"], context->fonts_["papyrus"], window, 0);

    // Waz 2
    CardStats snakeStats2 = tmp_deck.generateCardStats(CreatureType::OPOS);
    Card* opos = context->manager_->BuildCard(snakeStats2, context->textures_[snakeStats2.textureKey], context->textures_["card"], context->fonts_["papyrus"], window, 0);

    CardStats roachStats = tmp_deck.generateCardStats(CreatureType::KARALUCH);
    Card* roach = context->manager_->BuildCard(roachStats, context->textures_[roachStats.textureKey], context->textures_["card"], context->fonts_["papyrus"], window, 0);



    permanentDeck.emplace_back(card);
    permanentDeck.emplace_back(card1);
    permanentDeck.emplace_back(opos);
    permanentDeck.emplace_back(card3);
    permanentDeck.emplace_back(roach);
 

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
    TemplateDeck tmp_deck;
    CardStats squi = tmp_deck.generateCardStats(CreatureType::WIEWIORKA);
    Card* squirrel = manager->BuildCard(squi, manager->getTextureCreature(CreatureType::WIEWIORKA), manager->getTexture("card"), manager->getFont("papyrus"), manager->getWindow(),2);
    hand.push_back(squirrel);


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
    auto it = std::find(permanentDeck.begin(), permanentDeck.end(), c);
    if (it != permanentDeck.end()) {
        delete *it;
        permanentDeck.erase(it);
    }
}



