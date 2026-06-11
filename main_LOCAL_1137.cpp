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
//sf::Font font;

sf::Texture placeholder;
sf::Texture card;
sf::Texture cardReverse;
sf::Texture cardBackground;
sf::Texture cardSlot;




#include "Rendering.h"

Fonts* fonts=nullptr;

#include "Interactive.h"
#include "Card.h"
#include "SFML/Graphics/Sprite.hpp"
#include "Player.h"
#include "GameLog.h"




///lista wszystkich obiektow ktore mozna renderowac
std::vector<CustomDrawable*> Drawables;
std::vector<Card*> Cards;
#include "LogThread.h"

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


    fonts = Fonts::getInstance();
    //temporary way of loading font, should be done in Interface.cpp probably



    try{
        LoadTextures();

        if (!fonts->font.loadFromFile("resources/papyrus.ttf") && !fonts->font.loadFromFile("../../../resources/papyrus.ttf"))
            throw (std::string)"No font found";
    }
    catch(std::string string){
        std::cout<<string<<"\n";
        return 0;
    }

    DrawInterface(window);

    sf::Thread gLoop(&GameLoop, &window);
    gLoop.launch();


    sf::Thread logLoop(&LogLoop, &window);
    logLoop.launch();



    Card sprite(0);
    sprite.setTexture(card);
    sprite.setScale(0.2,0.2);
    sprite.setPosition(200,200);
    Cards.emplace_back(&sprite);
    sprite.window = &window;





    //tworzenie karty
    Card* c1 = BuildCard(2,5, 1, BLOOD, placeholder, 20);
    c1->setPosition(100,100);
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
                            //w przyszlosci bedziemy musieli chyba miec refrence do zaznaczonych obiektow
                            //zeby mnzna z zaznaczonymi obiektami cos robic, np. przemieszczac albo usuwac/poswiecac
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



        for(auto obj: Drawables){
            obj->Draw();



        }
        for(auto obj: Cards){
            obj->Draw();

        }


        window.draw(tmpTxt);

        window.display();
    }

    return 0;
}
