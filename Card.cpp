#include "Card.h"


Card* Card::clone() const {
    // Tworzymy zupełnie nowy obiekt na stercie i kopiujemy do niego obecne wartości
    Card* newCard = new Card(
        this->health,
        this->damage,
        this->cost,
        this->costType_,
        this->spriteTexture,
        this->window,
        0
        );

    return newCard;
}


// Card* BuildCard(int _damage, int _health, int _cost, CostType _ct,
//                 const sf::Texture& _texture, const sf::Texture& _background,
//                 const sf::Font& _font, sf::RenderWindow &window, int z=0)
// {
//     Card* c = new Card(_health, _damage, _cost, _ct, _texture, window, z);
//     c->background = _background;
//     c->setTexture(c->background);


//     c->spriteTexture = _texture;
//     CustomDrawable* cardSprite = new CustomDrawable();
//     cardSprite->setTexture(c->spriteTexture);
//     cardSprite->setRelativePosition(8, 36);
//     cardSprite->setScale(0.89,0.89);
//     c->addChild(cardSprite);


//     // std::cout << "spriteTexture size: " << c->spriteTexture.getSize().x << "x" << c->spriteTexture.getSize().y << "\n";
//     // std::cout << "cardSprite texture ptr: " << cardSprite->getTexture() << "\n";
//     // std::cout << "cardSprite textureRect: " << cardSprite->getTextureRect().width << "x" << cardSprite->getTextureRect().height << "\n";




//     CustomTextDrawable* health = new CustomTextDrawable(&(c->health));
//     health->setRelativePosition(45, 70);
//     health->text->setCharacterSize(24);
//     health->text->setFillColor(sf::Color::Black);
//     health->text->setFont(_font);
//     health->text->setOutlineThickness(1);
//     c->addChild(health);

//     CustomTextDrawable* damage = new CustomTextDrawable(&(c->damage));
//     damage->setRelativePosition(7, 70);
//     damage->text->setCharacterSize(24);
//     damage->text->setFillColor(sf::Color::Black);
//     damage->text->setFont(_font);
//     damage->text->setOutlineThickness(1);
//     c->addChild(damage);

//     CustomTextDrawable* cost = new CustomTextDrawable(&(c->cost));
//     cost->setRelativePosition(30, 15);
//     cost->text->setCharacterSize(12);
//     cost->text->setFillColor(sf::Color::Black);
//     cost->text->setFont(_font);
//     cost->text->setOutlineThickness(1);
//     c->addChild(cost);

//     if(_ct == CostType::BLOOD)
//         cost->after = " Blood";
//     else
//         cost->after = " Bones";

//     return c;

// }

// inline Card* cloneCard(const Card* c){
//     sf::RenderWindow& windowRef = const_cast<sf::RenderWindow&>(c->getWindow());
//     const sf::Texture* currentSpriteTex = c->sprite.getTexture();
//     return BuildCard(
//         c->getDamage(),
//         c->getHealth(),
//         c->getCost(),
//         c->getCostType(),
//         currentSpriteTex ? *currentSpriteTex : c->spriteTexture,
//         c->background,
//         *(c->font),
//         windowRef
//         );




//     //return BuildCard(c->health, c->damage, c->cost, c->costType_, *(c->sprite.getTexture(),c->getTexture(), c->getWindow()));
// }