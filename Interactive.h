#pragma once

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#endif // INTERACTIVE_H

#include "Rendering.h"

class Interactive: public CustomDrawable{
public:
    Interactive(int z): CustomDrawable(z){}
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
    sf::Color getOrginalColor(){
        return orginalColor;
    }
    sf::Color getSelectedColor(){
        return selectedColor;
    }
    void setOrginalColor(sf::Color color){
        orginalColor=color;
    }
private:
    sf::Color orginalColor= this->getColor();
    sf::Color selectedColor=sf::Color(230,230,50);
    bool selected=false;

};
