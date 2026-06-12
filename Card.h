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
    bool isMarkedForSacrifice_ = false;


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

    Card(int _health, int _damage, int _cost, CostType _ct, sf::Texture _texture, sf::RenderWindow* window, int z=0): Interactive(z, window){
        health = _health;
        damage = _damage;
        cost = _cost;
        costType_ = _ct;
        sprite.setTexture(_texture);
    }




    Card* clone() const;

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


    void setSacrificeHighlight(bool highlight) {
        isMarkedForSacrifice_ = highlight;
        if (isMarkedForSacrifice_) {
            // Kolorujemy kartę na czerwono-krwisty odcień
            this->setColor(sf::Color(255, 100, 100));
        } else {
            // Przywracamy domyślny, biały kolor (brak filtra)
            this->setColor(sf::Color::White);
        }
    }

    bool isMarkedForSacrifice() const { return isMarkedForSacrifice_;}


//private:
    int health;
    int damage;
    int cost;
    CostType costType_;

};



#endif // CARD_H
