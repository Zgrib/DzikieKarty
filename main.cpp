#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <chrono>


#include <SFML/System/Thread.hpp>
sf::Font font;

sf::Texture placeholder;
sf::Texture card;
sf::Texture cardReverse;
sf::Texture cardBackground;
sf::Texture cardSlot;



#include "Rendering.h"
#include "Interactive.h"
#include "Card.h"
#include "SFML/Graphics/Sprite.hpp"




///lista wszystkich obiektow ktore mozna renderowac
std::vector<CustomDrawable*> Drawables;

std::vector<Card*> Cards;


#include "Interface.cpp"

//może lepiej aby to były sloty na kartę a nie po prostu karty???
//to chyba zależy, tak też można
std::vector<std::vector<Card*>> board;

CustomDrawable * tempTest;

#include "GameThread.cpp"


Fonts* Fonts::instance = nullptr;
std::mutex Fonts::mtx;


int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Dzikie Karty");


    Fonts* fonts = Fonts::getInstance();

    //temporary way of loading font, should be done in Interface.cpp probably
    fonts->loadFont("../../../resources/papyrus.ttf");


    try{
        LoadTextures();
    }
    catch(std::string string){
        std::cout<<string<<"\n";
        return 0;
    }

    DrawInterface(window);

    sf::Thread gLoop(&GameLoop, &window);

    gLoop.launch();




    Card sprite(0);
    sprite.setTexture(card);
    sprite.setScale(0.2,0.2);
    sprite.setPosition(200,200);
    Cards.emplace_back(&sprite);
    sprite.window = &window;

    // Card sprite2(-1);
    // sprite2.setTexture(card);
    // sprite2.setScale(0.2,0.2);
    // sprite2.setPosition(500,100);
    // sprite2.setOrginalColor(sf::Color(200,100,0));
    // Cards.emplace_back(&sprite2);
    // sprite2.window = &window;


    // Card sprite3(10);
    // sprite3.setTexture(card);
    // sprite3.setScale(0.2,0.2);
    // sprite3.setPosition(100,150);
    // sprite3.setOrginalColor(sf::Color(100,200,0));
    // Cards.emplace_back(&sprite3);
    // sprite3.window = &window;




    //tworzenie karty
    Card* c1 = BuildCard(2,5,1,BLOOD,placeholder,20);
    c1->setPosition(100,100);
    Cards.emplace_back(c1);
    c1->window = &window;
    Cards.emplace_back(c1);







    //test relatywnych pozycji
    CustomDrawable sprite4(16);
    sprite4.setTexture(card);
    sprite4.setScale(0.05,0.05);
    sprite4.setColor(sf::Color(100,100,100));
    CustomDrawable test(15, &window);
    test.setTexture(cardBackground);
    test.setPosition(700,100);
    test.addChild(&sprite4, 20, 25);


    tempTest = &test;

    Drawables.emplace_back(&test);





    sf::Text tmpTxt;
    tmpTxt.setString("test test test cwel lorem ipsum");
    tmpTxt.setPosition(200,200);
    tmpTxt.setCharacterSize(24);
    tmpTxt.setFillColor(sf::Color::White);
    tmpTxt.setFont(fonts->font);
    tmpTxt.setOutlineThickness(2);

    std::sort(Drawables.begin(), Drawables.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});

    std::sort(Cards.begin(), Cards.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});

    for (auto obj: Drawables){
        obj->setWindow(&window);
    }



    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    std::cout << "Mouse clicked: " << mouse_pos.x << ", " << mouse_pos.y << std::endl;
                    for (auto &r: Cards) {
                        if (r->contains(mouse_pos)){
                            r->setSelected(!r->isSelected());
                        }

                    }
                }
            }

        }




        for( auto obj: Cards){
            if (obj->isSelected()){
                obj->setColor(obj->getSelectedColor());
            }
            else{
                obj->setColor(obj->getOrginalColor());
            }
        }


        // clear the window with black color
        window.clear(sf::Color::Black);


        //c1->Draw();



        for(auto obj: Drawables){
            //nowa metoda rysowania
            obj->Draw();

            //stara metoda rysowania
            //window.draw(*obj);


        }
        for(auto obj: Cards){

            //window.draw(*obj);
            obj->Draw();

        }


        window.draw(tmpTxt);

        window.display();
    }

    return 0;
}
