#ifndef CARD_H
#define CARD_H


#endif // CARD_H
enum CostType{
    BONES,
    BLOOD
};

class Card: public Interactive{
public:
    sf::Sprite sprite;
    sf::Sprite background;
    sf::Sprite reverse;

    sf::Vector2f globalPosition;
    Card(int z): Interactive(z){ //blank card
        health = 1;
        damage = 0;
        cost = 0;
        costType_ = BLOOD;
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


private:
    int health;
    int damage;
    int cost;
    CostType costType_;

};
