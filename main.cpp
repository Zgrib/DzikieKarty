#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>


#include "Rendering.h"


//moze warto by bylo zaimplementowac jakas metode sortowania wektora dla kontroli co sie renderuje szybciej
///lista wszystkich obiektow ktore mozna renderowac
//std::vector<sf::Drawable *> Objects;


sf::Texture placeholder;
sf::Texture card;



#include "Interface.cpp"





int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Dzikie Karty");
    //sf::View view;
    //view.reset(sf::FloatRect(0, 0, 1200, 800));
    //window.setView(view);
    std::vector<CustomDrawable*> Drawables;


    if (!card.loadFromFile("../../../resources/karta.png")) {
        std::cerr << "Could not load texture !" << std::endl;
        return 1;
    }

    if (!placeholder.loadFromFile("../../../resources/placeholder.png")) {
        std::cerr << "Could not load texture !" << std::endl;
        return 1;
    }


    CustomDrawable background(-10);
    background.setTexture(placeholder);
    background.setColor(sf::Color(200,141,60));
    background.setScale((float)window.getSize().x/(float)placeholder.getSize().x, (float)window.getSize().y/(float)placeholder.getSize().y);
    Drawables.emplace_back(&background);

    CustomDrawable leftPanel(-8);
    leftPanel.setTexture(placeholder);
    leftPanel.setColor(sf::Color(229,161,80));
    leftPanel.setScale((float)window.getSize().x*0.25/(float)placeholder.getSize().x, (float)window.getSize().y*0.7/(float)placeholder.getSize().y);
    Drawables.emplace_back(&leftPanel);


    CustomDrawable rightPanel(-8);
    rightPanel.setTexture(placeholder);
    rightPanel.setPosition(window.getSize().x*0.75,0);
    rightPanel.setColor(sf::Color(229,161,80));
    rightPanel.setScale((float)window.getSize().x*0.25/(float)placeholder.getSize().x, (float)window.getSize().y*0.7/(float)placeholder.getSize().y);
    Drawables.emplace_back(&rightPanel);

    CustomDrawable bottomPanel(-8);
    bottomPanel.setTexture(placeholder);
    bottomPanel.setOrigin(0,placeholder.getSize().y);
    bottomPanel.setPosition(0,window.getSize().y);
    bottomPanel.setColor(sf::Color(250,181,100));
    Drawables.emplace_back(&bottomPanel);
    bottomPanel.setScale((float)window.getSize().x/(float)placeholder.getSize().x, (float)window.getSize().y*0.3/(float)placeholder.getSize().y);





    CustomDrawable sprite(0);
    sprite.setTexture(card);
    sprite.setScale(0.2,0.2);
    sprite.setPosition(200,200);
    Drawables.emplace_back(&sprite);


    CustomDrawable sprite2(-1);
    sprite2.setTexture(card);
    sprite2.setScale(0.2,0.2);
    sprite2.setPosition(150,100);
    sprite2.setColor(sf::Color(200,100,0));
    Drawables.emplace_back(&sprite2);



    CustomDrawable sprite3(10);
    sprite3.setTexture(card);
    sprite3.setScale(0.2,0.2);
    sprite3.setPosition(100,150);
    sprite3.setColor(sf::Color(100,200,0));
    Drawables.emplace_back(&sprite3);



    DrawInterface();

    std::sort(Drawables.begin(), Drawables.end(), [](const  CustomDrawable* a, const  CustomDrawable* b){ return (a->z < b->z);});


    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        //sprite2.rotate(0.1);

        for(auto obj: Drawables){
            window.draw(*obj);
        }


        //ts rotates
        //view.rotate(0.1);
        //window.setView(view);

        window.display();
    }

    return 0;
}
