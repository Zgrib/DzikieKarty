#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>


#include "Rendering.h"
#include "Interactive.h"
#include "Card.h"


sf::Texture placeholder;
sf::Texture card;
sf::Texture cardReverse;
sf::Texture cardBackground;
sf::Texture cardSlot;

///lista wszystkich obiektow ktore mozna renderowac
std::vector<CustomDrawable*> Drawables;

std::vector<Card*> Cards;


#include "Interface.cpp"

//może lepiej aby to były sloty na kartę a nie po prostu karty???
//to chyba zależy, tak też można
std::vector<std::vector<Card*>> board;




/*
    Aktualnie glownie interfejsem by (moim zdaniem) warto bylo sie zajac,
    ale to jest git i version control, wiec mozna to robic rownolegle z innymi rzeczami.

    TODO: (to dopiero luzne plany na rozbudowe)


-skalować dynamicznie obiekty z klasy board, tak jak było na makiecie (3x4 albo 3x5)

-ogarnac jakas implementacje skalowania/pozycji relatywnych do innego obiektu. to duzo ulatwi w robieniu interfejsu, jakis freestyle najlepiej.

-reszta implementacji klasy Card-- konstruktor, destruktor i relatywne pozycje tekstu (i innych podelementow karty) wzgledem samej karty
    aby ruszanie karty bylo proste i bezbolesne. przykladowo card.setPosition(int x, int y), gdzie podelementy karty takie jak nazwa (tekst)
    sprite i statystyki (tekst) zostaly wzgledem karty nie ruszone

    na cholere taka rozprawke o tym napisalem? nie wiem

-interakcja z interfejsem? samo wykrywanie wystarczy chyba



*/

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Dzikie Karty");


    try{
        LoadTextures();
    }
    catch(std::string string){ //try catch bo tak chyba lepiej niz masa returnow
        std::cout<<string<<"\n";
        return 0;
    }

    DrawInterface(window);








    Card sprite(0);
    sprite.setTexture(card);
    sprite.setScale(0.2,0.2);
    sprite.setPosition(200,200);
    Cards.emplace_back(&sprite);


    Card sprite2(-1);
    sprite2.setTexture(card);
    sprite2.setScale(0.2,0.2);
    sprite2.setPosition(500,100);
    sprite2.setOrginalColor(sf::Color(200,100,0));
    Cards.emplace_back(&sprite2);



    Card sprite3(10);
    sprite3.setTexture(card);
    sprite3.setScale(0.2,0.2);
    sprite3.setPosition(100,150);
    sprite3.setOrginalColor(sf::Color(100,200,0));
    Cards.emplace_back(&sprite3);





    //test relatywnych pozycji
    CustomDrawable sprite4(16);
    sprite4.setTexture(card);
    sprite4.setScale(0.05,0.05);
    sprite4.setColor(sf::Color(100,100,100));
    CustomDrawable test(15, &window);
    test.setTexture(cardBackground);
    test.setPosition(700,100);
    test.addChild(&sprite4, 20, 25);

    //Drawables.emplace_back(test);



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
            if (obj->isSelected())
                obj->setColor(obj->getSelectedColor());
            else{
                    obj->setColor(obj->getOrginalColor());
            }
        }


        // clear the window with black color
        window.clear(sf::Color::Black);



        for(auto obj: Drawables){
            //nowa metoda rysowania
            obj->Draw();

            //stara metoda rysowania
            //window.draw(*obj);


        }
        for(auto obj: Cards){

            window.draw(*obj);

        }
        //ruszanie obiektu (dla testu)
        test.setPosition(test.getPosition().x-0.05,test.getPosition().y+0.02);

        //rysowanie rekursywne obiektu oraz jego dzeci
        test.Draw();

        window.display();
    }

    return 0;
}
