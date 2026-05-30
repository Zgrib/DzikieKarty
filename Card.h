#ifndef CARD_H
#define CARD_H


#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>
#endif // CARD_H
enum CostType{
    BONES,
    BLOOD
};

class Card: public Interactive{
public:
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


Card* BuildCard(int _health, int _damage, int _cost, CostType _ct, sf::Texture _texture, int z=0){
    Fonts* fonts = Fonts::getInstance();
    Card* c = new  Card(_health, _damage, _cost, _ct, _texture, z);
    c->background = cardBackground;
    c->setTexture(c->background);


    CustomTextDrawable* health = new CustomTextDrawable(&(c->health));
    health->setRelativePosition(5,80);
    c->addChild(health);
    health->text->setCharacterSize(24);
    health->text->setFillColor(sf::Color::Black);
    health->text->setFont(fonts->font);
    health->text->setOutlineThickness(1);


    CustomTextDrawable* damage = new CustomTextDrawable(&(c->damage));
    damage->setRelativePosition(65,80);
    c->addChild(damage);
    damage->text->setCharacterSize(24);
    damage->text->setFillColor(sf::Color::Black);
    damage->text->setFont(fonts->font);
    damage->text->setOutlineThickness(1);

    CustomTextDrawable* cost = new CustomTextDrawable(&(c->cost));
    cost->setRelativePosition(15,20);
    c->addChild(cost);
    cost->text->setCharacterSize(18);
    if(_ct==CostType::BLOOD)
        cost->after="Blood";
    else
        cost->after="Bones";
    cost->text->setFillColor(sf::Color::Black);
    cost->text->setFont(fonts->font);
    cost->text->setOutlineThickness(1);



    //to sie jako tekst renderuje??? xddd???
    CustomDrawable* sprite = new CustomDrawable();
    sprite->setTexture(_texture);
    c->addChild(sprite);
    sprite->setRelativePosition(10,30);













    return c;
}


