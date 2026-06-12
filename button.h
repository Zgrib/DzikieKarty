#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#endif

#include "Interactive.h"
#include <functional>

class Button : public Interactive
{
private:
    sf::Text buttonText;
    std::function<void()> onClickAction;

    enum ButtonState { Normal, Hover, Pressed };
    ButtonState state;

    sf::Color normalColor;
    sf::Color hoverColor;
    const sf::Texture* normalTexture;
    const sf::Texture* pressedTexture;
    bool isVisible_ = true;

public:
    Button(int z, const sf::Texture& texNormal, const sf::Texture& texPressed, const sf::Font& font, const std::string& text,sf::RenderWindow *_window);

    void setOnClickAction(std::function<void()> action);

    void update(sf::Vector2i mousePos, bool isMousePressed);

    void setVisible(bool visible);
    bool isVisible() const;
    std::string getTextString() const;

    virtual void Draw() override;
};
//to ma z czegoś dziedzićzyć nie pamietam, sory @zgrib
//done
