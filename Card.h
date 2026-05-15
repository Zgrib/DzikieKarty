#ifndef CARD_H
#define CARD_H

#endif // CARD_H
enum CostType{
    BONES,
    BLOOD
};

class Card: public CustomDrawable{
public:
    sf::Sprite sprite;
    sf::Sprite background;
    sf::Sprite reverse;

    sf::Vector2f globalPosition;

    int health;
    int damage;
    int cost;
    CostType costType;


    Card(){ //blank card
        health = 1;
        damage = 0;
        cost = 0;
        costType = BLOOD;


    }



};
