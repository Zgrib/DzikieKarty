#include "GameBoard.h"

// Zwraca pozycję lewego górnego rogu wybranego slotu
sf::Vector2f GameBoard::getSlotPosition(int row, int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        return sf::Vector2f(0.f, 0.f); // Zabezpieczenie przed wyjściem poza indeksy
    }

    float posX = bounds_.left + (col * slotWidth_);
    float posY = bounds_.top + (row * slotHeight_);

    return sf::Vector2f(posX, posY);
}

// Zwraca pozycję środka wybranego slotu
sf::Vector2f GameBoard::getSlotCenter(int row, int col) const {
    sf::Vector2f pos = getSlotPosition(row, col);
    return sf::Vector2f(pos.x + slotWidth_ / 2.f, pos.y + slotHeight_ / 2.f);
}