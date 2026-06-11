#ifndef CARD_H
#define CARD_H


#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Interactive.h"

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
    const sf::Font* font = nullptr;

    sf::Vector2f globalPosition;


    Card(int z, sf::RenderWindow &window): Interactive(z, &window){ //blank card
        health = 1;
        damage = 0;
        cost = 0;
        costType_ = BLOOD;

    }
    Card(int _health, int _damage, int _cost, CostType _ct, sf::Texture _texture, sf::RenderWindow &window, int z=0): Interactive(z, &window){
        health = _health;
        damage = _damage;
        cost = _cost;
        costType_ = _ct;
        sprite.setTexture(_texture);
    }

    ~Card() override {}

    int getHealth() const{
        return health;
    }
    int getDamage() const{
        return damage;
    }
    int getCost() const{
        return cost;
    }
    CostType getCostType() const{
        return costType_;
    }
    const sf::RenderWindow& getWindow() const{
        return (*window);
    }


//private:
    int health;
    int damage;
    int cost;
    CostType costType_;

};



#endif // CARD_H
