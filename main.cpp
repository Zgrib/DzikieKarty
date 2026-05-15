#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>


#include "Rendering.h"
#include "Card.h"



sf::Texture placeholder;
sf::Texture card;
sf::Texture cardReverse;
sf::Texture cardBackground;
sf::Texture cardSlot;

///lista wszystkich obiektow ktore mozna renderowac
std::vector<CustomDrawable*> Drawables;

#include "Interface.cpp"

std::vector<std::vector<Card*>> board;


int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Dzikie Karty");
    //sf::View view;
    //view.reset(sf::FloatRect(0, 0, 1200, 800));
    //window.setView(view);

    try{
        LoadTextures();
    }
    catch(std::string string){
        std::cout<<string<<"\n";
        return 0;
    }
    DrawInterface(window);








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



    //DrawInterface();

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
