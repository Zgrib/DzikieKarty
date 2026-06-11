#include "button.h"

Button::Button(int z, const sf::Texture& texNormal, const sf::Texture& texPressed, const sf::Font& font, const std::string& text,sf::RenderWindow *_window)
    : Interactive(z,_window), state(Normal)
{
    normalTexture = &texNormal;
    pressedTexture = &texPressed;

    this->setTexture(*normalTexture);

    // presety do przyceimniania
    normalColor = sf::Color(255, 255, 255);
    hoverColor = sf::Color(200, 200, 200);
    this->setColor(normalColor);


    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setOutlineColor(sf::Color::Black);
    buttonText.setOutlineThickness(2);

    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);


    sf::FloatRect buttonRect = this->getLocalBounds();
    buttonText.setPosition(buttonRect.width / 2.0f, buttonRect.height / 2.0f);
}

void Button::setOnClickAction(std::function<void()> action)
{
    onClickAction = action;
}

void Button::update(sf::Vector2i mousePos, bool isMousePressed)
{
    if (contains(mousePos))
    {
        if (isMousePressed)
        {
            state = Pressed;
            this->setColor(hoverColor);
            this->setTexture(*pressedTexture);  // trzeba zrobić zamianę na button_pressed
        }
        else
        {

            if (state == Pressed && onClickAction != nullptr)
            {
                onClickAction();
            }
            state = Hover;
            this->setColor(hoverColor); // przyciemnienie
            this->setTexture(*normalTexture); //zmiana na normalny np po przyciśnięciu
    }
    }
    else
    {
        state = Normal;
        this->setColor(normalColor); // orginał
        this->setTexture(*normalTexture);
    }
}

void Button::Draw()
{

    CustomDrawable::Draw();

    if (window != nullptr)
    {
        sf::RenderStates states;
        states.transform = this->getTransform();
        buttonText.setScale(0.7,0.7);
        window->draw(buttonText, states);

    }
}
