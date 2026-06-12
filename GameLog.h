#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <deque>
#include "Rendering.h"

class GameLog : public CustomDrawable {
private:
    sf::Text title;
    sf::Text logText;
    std::deque<std::string> messages;
    int maxLines = 17; // ilosc linii do odciecia 15 dla testy, do sprawdzenia
    // 14 idealnie przy 25 czcionce
    static GameLog* instance; // statyczna instancja zeby wywołać znikąd

public:
    GameLog(const sf::Font& font, sf::RenderWindow* window);

    // może zadziala
    static void add(const std::string& msg);

    static void clear();

    virtual void Draw() override;
};