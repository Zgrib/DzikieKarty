#include "Player.h"

void Player::addCard(const Card* c){
    //deck.emplace_back(cloneCard(c));
}


std::vector<Card> Player::cloneDeck(){
    //bedzie uzywane na poczatku bitwy do stworzenia "roboczego" decku ktory
    //bedziemy mogli tasowac, usuwac z niego karty itd...
    // std::vector<Card> cards;
    // for(const auto card:deck){
    //     Card* tmp = cloneCard(card);
    //     cards.emplace_back(*tmp);
    //     delete tmp;
    // }
    // return cards;

}
void Player::prepareForBattle() {
    battleDeck.clear();
    hand.clear();

    // Klonujemy wskaźniki (głęboka kopia, aby modyfikacje HP w bitwie nie niszczyły bazy)
    for (const Card* baseCard : permanentDeck) {
        // Zakładam, że masz metodę manager_->cloneCard lub card->clone()
        // battleDeck.push_back(baseCard->clone());
    }

    // Na start wyciągamy np. 3 karty na rękę
    for (int i = 0; i < 3; ++i) {
        drawCard();
    }
}

void Player::drawCard() {
    if (!battleDeck.empty()) {
        Card* drawn = battleDeck.back();
        battleDeck.pop_back();
        hand.push_back(drawn);
        updateHandPositions(); // Odświeżenie pozycji graficznych na pasku
    }
}

void Player::removeCard(Card* c) {
    auto it = std::find(deck.begin(), deck.end(), c);
    if (it != deck.end()) {
        delete *it;
        deck.erase(it);
    }
}


void Player::updateHandPositions() {
    if (hand.empty()) return;

    float screenWidth = 1920.f;  // Dopasuj do swojej rozdzielczości bazowej
    float screenHeight = 1080.f;

    float cardWidth = hand[0]->background.getSize().x * hand[0]->getScale().x;
    float spacing = 15.f; // Odstęp między kartami w ręce
    float handY = screenHeight - (hand[0]->background.getSize().y * hand[0]->getScale().y) - 20.f; // 20px od dołu

    // Obliczamy całkowitą szerokość całej ręki, aby ją idealnie wycentrować
    float totalWidth = (hand.size() * cardWidth) + ((hand.size() - 1) * spacing);
    float startX = (screenWidth - totalWidth) / 2.f;

    for (size_t i = 0; i < hand.size(); ++i) {
        float finalX = startX + i * (cardWidth + spacing);
        hand[i]->setPosition(finalX, handY);
    }
}

