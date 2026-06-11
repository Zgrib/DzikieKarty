#pragma once

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <SFML/Graphics.hpp>

class GameBoard {
private:
    sf::FloatRect bounds_;
    int rows_;
    int cols_;

    float slotWidth_;
    float slotHeight_;

public:
    // Konstruktor (może pozostać w nagłówku jako inline)
    GameBoard(sf::FloatRect bounds, int rows = 2, int cols = 4)
        : bounds_(bounds), rows_(rows), cols_(cols) {
        slotWidth_ = bounds_.width / cols_;
        slotHeight_ = bounds_.height / rows_;
    }

    // Deklaracje metod (implementacja w pliku .cpp)
    sf::Vector2f getSlotPosition(int row, int col) const;
    sf::Vector2f getSlotCenter(int row, int col) const;

    // Gettery inline
    float getSlotWidth() const { return slotWidth_; }
    float getSlotHeight() const { return slotHeight_; }
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
};

#endif // GAMEBOARD_H