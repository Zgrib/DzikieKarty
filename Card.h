#ifndef CARD_H
#define CARD_H


#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>

enum CostType{
    BONES,
    BLOOD
};

class Card: public Interactive{
public:
    sf::Texture spriteTexture;
    sf::Sprite sprite;
    sf::Texture background;
    sf::Texture reverse;

    sf::Vector2f globalPosition;

    Card(int z): Interactive(z){ //blank card
        health = 1;
        damage = 0;
        cost = 0;
        costType_ = BLOOD;

    }
    Card(int _health, int _damage, int _cost, CostType _ct, sf::Texture _texture, int z=0): Interactive(z){
        health = _health;
        damage = _damage;
        cost = _cost;
        costType_ = _ct;
        sprite.setTexture(_texture);
    }

    int getHealth(){
        return health;
    }
    int getDamage(){
        return damage;
    }
    int getCost(){
        return cost;
    }
    CostType getCostType(){
        return costType_;
    }



//private:
    int health;
    int damage;
    int cost;
    CostType costType_;

};


Card* BuildCard(int _damage, int _health, int _cost, CostType _ct, const sf::Texture& _texture, int z=0){
    Fonts* fonts = Fonts::getInstance();
    Card* c = new Card(_health, _damage, _cost, _ct, _texture, z);
    c->background = cardBackground;
    c->setTexture(c->background);


    c->spriteTexture = _texture;
    CustomDrawable* cardSprite = new CustomDrawable();
    cardSprite->setTexture(c->spriteTexture);
    cardSprite->setRelativePosition(8, 36);
    cardSprite->setScale(0.89,0.89);
    c->addChild(cardSprite);


    // std::cout << "spriteTexture size: " << c->spriteTexture.getSize().x << "x" << c->spriteTexture.getSize().y << "\n";
    // std::cout << "cardSprite texture ptr: " << cardSprite->getTexture() << "\n";
    // std::cout << "cardSprite textureRect: " << cardSprite->getTextureRect().width << "x" << cardSprite->getTextureRect().height << "\n";




    CustomTextDrawable* health = new CustomTextDrawable(&(c->health));
    health->setRelativePosition(45, 70);
    health->text->setCharacterSize(24);
    health->text->setFillColor(sf::Color::Black);
    health->text->setFont(fonts->font);
    health->text->setOutlineThickness(1);
    c->addChild(health);

    CustomTextDrawable* damage = new CustomTextDrawable(&(c->damage));
    damage->setRelativePosition(7, 70);
    damage->text->setCharacterSize(24);
    damage->text->setFillColor(sf::Color::Black);
    damage->text->setFont(fonts->font);
    damage->text->setOutlineThickness(1);
    c->addChild(damage);

    CustomTextDrawable* cost = new CustomTextDrawable(&(c->cost));
    cost->setRelativePosition(30, 15);
    cost->text->setCharacterSize(12);
    cost->text->setFillColor(sf::Color::Black);
    cost->text->setFont(fonts->font);
    cost->text->setOutlineThickness(1);
    c->addChild(cost);

    if(_ct == CostType::BLOOD)
        cost->after = " Blood";
    else
        cost->after = " Bones";

    return c;

}

Card* cloneCard(const Card* c){
    return BuildCard(c->health, c->damage, c->cost, c->costType_, *(c->sprite.getTexture()));
}
#endif // CARD_H
