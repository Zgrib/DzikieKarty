#pragma once

#ifndef RENDERING_H
#define RENDERING_H

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include <iostream>
#include <string>
#endif // RENDERING_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <mutex>

class Fonts{
private:
    static Fonts* instance;
    static std::mutex mtx;
    Fonts(){}


public:

    sf::Font font;
    Fonts(const Fonts& obj) = delete;

    static Fonts* getInstance(){
        if(instance==nullptr){
            std::lock_guard<std::mutex> lock(mtx);
            if(instance==nullptr)
                instance = new Fonts();
        }

        return instance;
    }

    void loadFont(const std::string& path){
        if (!font.loadFromFile(path))
            std::cout<<"no fucking font!!!\n";
        //font.loadFromFile(path);

    }


};







///argument konstruktora stanowi kolejnosc renderowania, im mniejsze z, tym "niżej" jest sprite
class CustomDrawable:public sf::Sprite{


private:
    int z;

protected:
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
    virtual void Draw(){

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
    void setRelativePosition(float x, float y){
        positionRelative = sf::Vector2f(x,y);
    }

    sf::Vector2f getRelativePosition(){
        return positionRelative;
    }

    void addChild(CustomDrawable* child){
        children.emplace_back(child);
        child->parent = this;
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



class CustomTextDrawable:public CustomDrawable{
private:


public:
    //text publiczny zeby nie reimplementowac szystkich funkcji textu
    sf::Text* text;
    int* value=nullptr;
    std::string before;
    std::string after;


    CustomTextDrawable(){
        text = new sf::Text();
        text->setPosition(0,0);

    }


    CustomTextDrawable(int* _value){
        value= _value;
        text = new sf::Text();
        text->setPosition(0,0);
        //text->setFont(font);
        //text->setFillColor(sf::Color(0,0,0));
    }


    void Draw() override {
        if(parent!=nullptr){
            setPosition(parent->getPosition() + positionRelative);
            window = parent->window;
            text->setPosition(getPosition()+positionRelative);
        }
        if(value != nullptr)
            text->setString(before + std::to_string( *value ) + after);

        if(window!=nullptr){

            window->draw(*text);
            for(auto child:children){
                child->Draw();
            }

        }
    }
};
