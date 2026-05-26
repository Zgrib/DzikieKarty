#ifndef RENDERING_H
#define RENDERING_H

#endif // RENDERING_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>



///argument konstruktora stanowi kolejnosc renderowania, im mniejsze z, tym "niżej" jest sprite
class CustomDrawable:public sf::Sprite{


private:
    int z;
    std::vector<CustomDrawable*> children;
    sf::Vector2f positionRelative= sf::Vector2f(0,0);

public:
    ///wartosc z to kolejnosc rysowania obiektow
    sf::RenderWindow* window=nullptr;
    CustomDrawable* parent=nullptr;


    CustomDrawable(int _z = 0){
        z=_z;
    }
    CustomDrawable(int _z, sf::RenderWindow* _window){
        z=_z;
        window = _window;
    }
    CustomDrawable(int _z, sf::RenderWindow* _window, CustomDrawable* _parent){
        z=_z;
        window = _window;
        parent = _parent;
    }
    void Draw(){

        if(parent!=nullptr){
            setPosition(parent->getPosition() + positionRelative);
            window = parent->window;
        }

        if(window!=nullptr){


            window->draw(*this);

            for(auto child:children){
                child->Draw();
            }

        }
    }
    void setWindow(sf::RenderWindow* _window){
        window = _window;
        for(auto child:children)
            child->setWindow(_window);


    }

    void setRelativePosition(sf::Vector2f posRel){
        positionRelative = posRel;
    }

    sf::Vector2f getRelativePosition(){
        return positionRelative;
    }

    void addChild(CustomDrawable* child){
        children.emplace_back(child);
    }

    void addChild(CustomDrawable* child, float relX, float relY){
        children.emplace_back(child);
        child->setRelativePosition(sf::Vector2f(relX, relY));
        child->parent = this;
    }

    const int &getZ(){
        return z;
    }





};
