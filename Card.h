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
    Card(int z): CustomDrawable(z){ //blank card
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

    bool contains(sf::Vector2i pos){
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if (pos.x < rectangle_bounds.left)
            return false;
        if (pos.x > rectangle_bounds.left + rectangle_bounds.width)
            return false;
        if (pos.y < rectangle_bounds.top)
            return false;
        if (pos.y > rectangle_bounds.top + rectangle_bounds.height)
            return false;
        return true;
    }
    bool isSelected() const {
        return selected;
    }
    void setSelected(bool selected) {
        this->selected = selected;
    }
private:
    int health;
    int damage;
    int cost;
    CostType costType_;
    bool selected=false;

};
