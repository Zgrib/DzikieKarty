#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#endif // BUTTON_H

#include "Interactive.h"
#include <functional>

class Button : public Interactive
{
private:
    sf::Text buttonText;
    std::function<void()> onClickAction; // Funkcja, która odpali się po kliknięciu

    // Stany przycisku
    enum ButtonState { Normal, Hover, Pressed };
    ButtonState state;

    // Kolory do przyciemniania tekstury .png
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;

public:
    // Konstruktor: przyjmuje 'z', teksturę tła, czcionkę i tekst do wyświetlenia
    Button(int z, const sf::Texture& texture, const sf::Font& font, const std::string& text,sf::RenderWindow *_window);

    // Przypisanie akcji do przycisku
    void setOnClickAction(std::function<void()> action);

    // Metoda, którą wywołujesz w pętli gry, żeby przycisk reagował na myszkę
    void update(sf::Vector2i mousePos, bool isMousePressed);

    // Nadpisujemy Draw, żeby narysować najpierw tło (CustomDrawable), a potem tekst
    virtual void Draw() override;
};
//to ma z czegoś dziedzićzyć nie pamietam, sory @zgrib
