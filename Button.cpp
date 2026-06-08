#include "Button.h"

Button::Button(int z, const sf::Texture& texture, const sf::Font& font, const std::string& text)
    : Interactive(z), state(Normal)
{
    // 1. Ustawienie tekstury (CustomDrawable dziedziczy po sf::Sprite)
    this->setTexture(texture);

    // 2. Ustawienie kolorów dla efektu "podświetlenia"
    normalColor = sf::Color(255, 255, 255); // Zwykły, oryginalny kolor z .png
    hoverColor = sf::Color(200, 200, 200);  // Lekko przyciemniony (gdy myszka najedzie)
    pressedColor = sf::Color(120, 120, 120); // Mocno przyciemniony (gdy klikniemy)
    this->setColor(normalColor);

    // 3. Konfiguracja tekstu (Papyrus)
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White); // Biały tekst
    buttonText.setOutlineColor(sf::Color::Black); // Czarna obwódka dla lepszej czytelności
    buttonText.setOutlineThickness(2);

    // 4. Centrowanie tekstu idealnie na środku tekstury
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    // Ustawienie tekstu na środku przycisku (względem jego rozmiarów z .png)
    sf::FloatRect buttonRect = this->getLocalBounds();
    buttonText.setPosition(buttonRect.width / 2.0f, buttonRect.height / 2.0f);
}

void Button::setOnClickAction(std::function<void()> action)
{
    onClickAction = action;
}

void Button::update(sf::Vector2i mousePos, bool isMousePressed)
{
    // Funkcja contains() pochodzi z Twojego Interactive.h
    if (contains(mousePos))
    {
        if (isMousePressed)
        {
            state = Pressed;
            this->setColor(pressedColor); // Przyciemniamy
        }
        else
        {
            // Jeśli gracz właśnie PUSCIŁ myszkę (czyli był wciśnięty, a teraz nie jest)
            // To znaczy, że wykonał pełen "klik"
            if (state == Pressed && onClickAction != nullptr)
            {
                onClickAction(); // Wywołujemy naszą tajną funkcję!
            }
            state = Hover;
            this->setColor(hoverColor); // Lekkie przyciemnienie
        }
    }
    else
    {
        state = Normal;
        this->setColor(normalColor); // Wracamy do oryginału
    }
}

void Button::Draw()
{
    // 1. Najpierw wywołujemy bazową funkcję Draw (narysuje .png i zaktualizuje pozycje)
    CustomDrawable::Draw();

    // 2. Jeśli przycisk ma okno, rysujemy tekst precyzyjnie NA przycisku
    if (window != nullptr)
    {
        sf::RenderStates states;
        states.transform = this->getTransform(); // Tekst przesuwa się razem z tłem
        window->draw(buttonText, states);
    }
}