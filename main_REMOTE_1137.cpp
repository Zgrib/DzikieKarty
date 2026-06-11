#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <SFML/System/Thread.hpp>
sf::Font font;

sf::Texture placeholder;
sf::Texture card;
sf::Texture cardReverse;
sf::Texture cardBackground;
sf::Texture cardSlot;
sf::Texture wilk;
sf::Texture waz;
sf::Texture wiewiorka;
sf::Texture raven;


#include "Rendering.h"
#include "Interactive.h"
#include "Card.h"
#include "Deck.h"
#include "TemplateDeck.h"
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
    fonts->loadFont("resources/papyrus.ttf");


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
    Card* c1 = BuildCard(2,5,1,BLOOD,raven,20);
    c1->setPosition(100,00);
    Cards.emplace_back(c1);
    c1->window = &window;
    Cards.emplace_back(c1);




    //test relatywnych pozycji
    CustomDrawable sprite4(16);
    sprite4.setTexture(raven);
    sprite4.setScale(0.5,0.5);
    sprite4.setColor(sf::Color(100,100,100));
    CustomDrawable test(15, &window);
    test.setTexture(cardBackground);
    test.setPosition(700,100);
    test.addChild(&sprite4, 20, 25);

    Deck player_deck= Deck(100,300);
        player_deck.addCard(c1,Owner::Player);
         player_deck.addCard(c1,Owner::Player);
          player_deck.addCard(c1,Owner::Player);
    std::unordered_map<std::string, sf::Texture> card_map;
    card_map.emplace("wilk",card);
    card_map.emplace("waz",waz);
    card_map.emplace("wrona",raven);
    TemplateDeck tmp_deck= TemplateDeck(card_map);

    Card* wolf= tmp_deck.getRandomCard(CreatureType::WILK);
    Card* raven_= tmp_deck.getRandomCard(CreatureType::WRONA);
    Card* snake= tmp_deck.getRandomCard(CreatureType::WEZ);

      Cards.emplace_back(wolf);
      Cards.emplace_back(snake);
     Cards.emplace_back(raven_);

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
        player_deck.render(window);
        window.draw(tmpTxt);

        window.display();
    }

    return 0;
}
