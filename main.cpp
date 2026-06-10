#include "context.h"

int main(){
    Context DzikieKarty= Context();
    DzikieKarty.start_context();
    return 0;
}

//#include <iostream>
//#include <SFML/Window.hpp>
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <algorithm>
//#include <chrono>
//#include <unordered_map>
//#include <SFML/System/Thread.hpp>
//sf::Font font;

//sf::Texture placeholder;
//sf::Texture card;
//sf::Texture cardReverse;
//sf::Texture cardBackground;
//sf::Texture cardSlot;
//sf::Texture wilk;
//sf::Texture waz;
//sf::Texture wiewiorka;
//sf::Texture raven;
//sf::Texture Battleground;

//#include "Rendering.h"

//const float TIMESTEP = 0.1;
//Fonts* fonts=nullptr;

//#include "Interactive.h"
//#include "Card.h"
//#include "Deck.h"
//#include "TemplateDeck.h"
//#include "SFML/Graphics/Sprite.hpp"
//#include "Player.h"
//#include "GameLog.h"
//#include "BattleEngine.h"
//#include "Button.h"



/////lista wszystkich obiektow ktore mozna renderowac
//std::vector<CustomDrawable*> Drawables;

//std::vector<Card*> Cards;
//#include "LogThread.cpp"

//#include "Interface.cpp"

//CustomDrawable * tempTest;

//#include "GameThread.cpp"


//Fonts* Fonts::instance = nullptr;
//std::mutex Fonts::mtx;


//int main() {
//    // create the window
//    sf::RenderWindow window(sf::VideoMode(1920, 1800), "Dzikie Karty");



//    Fonts* fonts = Fonts::getInstance();

//    //temporary way of loading font, should be done in Interface.cpp probably
//    fonts->loadFont("resources/papyrus.ttf");


//    try{
//        LoadTextures();
//    }
//    catch(std::string string){
//        std::cout<<string<<"\n";
//        return 0;
//    }

//    DrawInterface(window);

//    sf::Thread gLoop(&GameLoop, &window);

//    gLoop.launch();


//    sf::Thread logLoop(&LogLoop, &window);
//    logLoop.launch();



//    Card sprite(0);
//    sprite.setTexture(card);
//    sprite.setScale(0.2,0.2);
//    sprite.setPosition(200,200);
//    Cards.emplace_back(&sprite);
//    sprite.window = &window;



//    //tworzenie karty
//    Card* c1 = BuildCard(2,3,2,BLOOD,raven,20);
//    c1->setPosition(300,200);
//    c1->setWindow(&window);
//    Cards.emplace_back(c1);
//    Card* c2 = BuildCard(3,1,2,BLOOD,wilk,20);
//    c2->setPosition(500,200);
//    c2->setWindow(&window);
//    Cards.emplace_back(c2);
//    Card* c3 = BuildCard(2,3,2,BLOOD,raven,20);
//    c3->setPosition(1000,200);
//    c3->setWindow(&window);
//    Cards.emplace_back(c3);


//    Deck player_deck=Deck(480,720,300,10);






//    // TEST PRZYCISKU
//    Button* Button1 = new Button(100, placeholder, fonts->font, "ELO");
//    Button1->setPosition(500, 400);

//    Button1->setOnClickAction([]() {
//        std::cout << "COS SE ROBIE Kurde TEN" << std::endl;
//    });

//    Drawables.push_back(Button1);
//    // TEST PRZYCISKU


//    // tempTest = &test;
//    // Drawables.emplace_back(&test);



//    sf::Text tmpTxt;
//    tmpTxt.setString("test test test więzienny homoseksualista lorem ipsum");
//    tmpTxt.setPosition(200,200);
//    tmpTxt.setCharacterSize(24);
//    tmpTxt.setFillColor(sf::Color::White);
//    tmpTxt.setFont(fonts->font);
//    tmpTxt.setOutlineThickness(2);

//    std::sort(Drawables.begin(), Drawables.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});

//    std::sort(Cards.begin(), Cards.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});

//    for (auto obj: Drawables){
//        obj->setWindow(&window);
//    }
//    for (auto obj: Cards){
//        obj->setWindow(&window);
//    }


//    // run the program as long as the window is open
//    while (window.isOpen()) {
//        // check all the window's events that were triggered since the last iteration of the loop
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            // "close requested" event: we close the window
//            if (event.type == sf::Event::Closed)
//                window.close();
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if(event.mouseButton.button == sf::Mouse::Left) {
//                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
//                    std::cout << "Mouse clicked: " << mouse_pos.x << ", " << mouse_pos.y << std::endl;
//                    for (auto &r: Cards) {
//                        if (r->contains(mouse_pos)){
//                            r->setSelected(!r->isSelected());
//                            //w przyszlosci bedziemy musieli chyba miec refrence do zaznaczonych obiektow
//                            //zeby mnzna z zaznaczonymi obiektami cos robic, np. przemieszczac albo usuwac/poswiecac
//                        }

//                    }
//                }
//            }

//        }




//        for( auto obj: Cards){
//            if (obj->isSelected()){
//                obj->setColor(obj->getSelectedColor());
//            }
//            else{
//                obj->setColor(obj->getOrginalColor());
//            }
//        }

//        // TEST PRZYCISKU
//        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
//        bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
//        Button1->update(currentMousePos, isLeftPressed);
//        // TEST PRZYCISKU

//        window.clear(sf::Color::White);


//        for(auto obj: Drawables){
//            obj->Draw();
//        }
//        for(auto obj: Cards){
//            obj->Draw();
//        }
//        player_deck.render(window);

//        //rysowanie w ten sposob kart rysuje jednynie ich tło!!

//        window.draw(tmpTxt);

//        window.display();
//    }

//    return 0;
//}
