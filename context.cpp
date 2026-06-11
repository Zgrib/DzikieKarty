#include "context.h"
#include "button.h"
#include <iostream>

Context::Context()
{

}
void Context::load_textures() {
    // list of name -> filename (adjust paths as needed)
    const std::vector<std::pair<std::string, std::string>> files = {
        {"placeholder", "resources/placeholder.png"},
        {"card",        "resources/karta.png"},
        {"phSlot",   "resources/phSlot.png"},
        {"bat", "resources/nietoperz.png"},
        {"wolf",        "resources/wilk.png"},
        {"snake",         "resources/waz.png"},
        {"squirel",   "resources/wiewiorka.png"},
        {"raven",       "resources/raven.png"},
        {"Battleground","resources/Battleground.png"},
        {"deck",       "resources/Deck.png"}
    };

    for (auto &p : files) {
        const std::string &key = p.first;
        const std::string &path = p.second;

        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            // try alternate relative path
            std::string alt = "../../../" + path;
            if (!tex.loadFromFile(alt)) {
                throw std::string("Could not load texture: ") + path;
            }
        }

        // insert into map (move avoids copy)
        textures_.emplace(key, std::move(tex));
    }
}
void Context::load_fonts(){
    const std::vector<std::pair<std::string, std::string>> files = {
        {"papyrus", "resources/papyrus.ttf"} // .ttf
    };

    for (auto &p : files) {
        const std::string &key = p.first;
        const std::string &path = p.second;

        sf::Font font;
        if (!font.loadFromFile(path)) {
            std::string alt = "../../../" + path;
            if (!font.loadFromFile(alt)) {
                throw std::string("Could not load font: ") + path;
            }
        }

        fonts_.emplace(key, std::move(font));
    }
}
void Context::start_context() {
    try {
        load_textures();
        load_fonts();
    } catch (const std::string &e) {
        std::cerr << e << '\n';
        return;
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Dzikie Karty");
    window.setFramerateLimit(60);

    manager_ = new GameManager(window);

    Context::start_main_menu(window);
    Context::start_battleground(window);

    //manager test!!
    Card* card = manager_->BuildCard(2,5,2,CostType::BLOOD,textures_["raven"],textures_["card"],fonts_["papyrus"],window,0);
    manager_->placeCard(card,0,0);

    Card* card3 = manager_->BuildCard(3,2,2,CostType::BLOOD,textures_["wolf"],textures_["card"],fonts_["papyrus"],window,0);
    manager_->placeCard(card3,1,0);

    Card* card1 = manager_->BuildCard(1,3,1,CostType::BLOOD,textures_["snake"],textures_["card"],fonts_["papyrus"],window,0);
    manager_->placeCard(card1,1,2);

    Card* card2 = manager_->BuildCard(1,3,1,CostType::BLOOD,textures_["snake"],textures_["card"],fonts_["papyrus"],window,0);
    manager_->placeCard(card2,0,3);


    //end of manager test

    Context::window_loop(window);

}

void Context::window_loop(sf::RenderWindow &window){
    sf::Clock deltaClock;
    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        Context::events_loop(window,event);

        // clear the window with black color
        window.clear(sf::Color::Red);

        // draw everything here...
        if(scene_== 0){
            Context::main_menu(window);    
        }
        if(scene_==1){
            manager_->getBattleEngine().update(deltaTime);
            Context::battleground(window);
            manager_->drawBoardElements();
        }

        // end the current frame
        window.display();
    }
}

void Context::events_loop(sf::RenderWindow &window,sf::Event &event){
    while (window.pollEvent(event)) {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed){
            window.close();
        }


/*       if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
           window.close();
       for(Button* ptr: menu_buttons_){
           ptr->update(sf::Vector2i(0,0),false);
       }
                   if (event.type == sf::Event::MouseButtonPressed) {
                       if(event.mouseButton.button == sf::Mouse::Left) {
                           sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                           for(Button* ptr: menu_buttons_){
                               ptr->update(mouse_pos,true);
                           }
                       }

                               sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                               bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                               Button1->update(currentMousePos, isLeftPressed);
                               // TEST PRZYCISKU
   }*/
    }
}

void Context::start_main_menu(sf::RenderWindow &window){
        Button* btn_start=new Button(5,textures_["phSlot"],fonts_["papyrus"],"START",&window);
        btn_start->setPosition(200,200);
        btn_start->setOnClickAction([this](){scene_=1;});
        menu_buttons_.emplace_back(btn_start);


        Button* btn_quit=new Button(5,textures_["phSlot"],fonts_["papyrus"],"WYJDZ",&window);
        btn_quit->setPosition(500,200);
        btn_quit->setOnClickAction([&window](){std::cout<<"Guzik"<<std::endl; window.close();});
        menu_buttons_.emplace_back(btn_quit);

        CustomDrawable* background= new CustomDrawable(-10);
        background->setTexture(textures_["Battleground"]);
        background->setColor(sf::Color(255,255,255));
        background->setScale((float)window.getSize().x/(float)textures_["Battleground"].getSize().x, (float)window.getSize().y/(float)textures_["Battleground"].getSize().y);
        menu_drawables_.emplace_back(background);



        std::sort(menu_drawables_.begin(), menu_drawables_.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});
        for(CustomDrawable* ptr: menu_drawables_){
            ptr->setWindow(&window);
        }
}
void Context::main_menu(sf::RenderWindow &window){
    for(CustomDrawable* ptr: menu_drawables_){
        ptr->Draw();
    }

    for(Button* ptr: menu_buttons_){
        ptr->Draw();
    }
    // in window_loop (each frame)
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    for (Button* btn : menu_buttons_) btn->update(mousePos, isPressed);

}


void Context::start_battleground(sf::RenderWindow &window){
    CustomDrawable* background= new CustomDrawable(-10);
    CustomDrawable* leftPanel= new CustomDrawable(-8);
    CustomDrawable* rightPanel= new CustomDrawable(-8);
    CustomDrawable* bottomPanel = new CustomDrawable(-8);

    Button* quit = new Button(5,textures_["phSlot"],fonts_["papyrus"],"WYJDZ",&window);
    quit->setPosition(1800,50);
    quit->setOnClickAction([this](){scene_=0;});//and maybe do other stuff?
    battle_buttons_.emplace_back(quit);
    std::cout<<"test test test\n";

    Button* endTurn = new Button(5,textures_["phSlot"],fonts_["papyrus"],"Zakoncz ture",&window);
    endTurn->setPosition(200,500);
    endTurn->setOnClickAction([this](){
        std::cout << "Koniec tury! Odpalam silnik...\n";
        manager_->getBattleEngine().EndTurn();
    });
    battle_buttons_.emplace_back(endTurn);
    std::cout << "Test/n";

    background->setTexture(textures_["Battleground"]);
    background->setColor(sf::Color(200,141,60));
    background->setPosition((float)window.getSize().x*0.25,0);
    background->setScale((float)window.getSize().x*0.5/(float)textures_["Battleground"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["Battleground"].getSize().y);
    battle_drawables_.emplace_back(background);



    leftPanel->setTexture(textures_["placeholder"]);
    leftPanel->setColor(sf::Color(229,161,80));
    leftPanel->setScale((float)window.getSize().x*0.25/(float)textures_["placeholder"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["placeholder"].getSize().y);
    battle_drawables_.emplace_back(leftPanel);



    rightPanel->setTexture(textures_["placeholder"]);
    rightPanel->setPosition(window.getSize().x*0.75,0);
    rightPanel->setColor(sf::Color(229,161,80));
    rightPanel->setScale((float)window.getSize().x*0.25/(float)textures_["placeholder"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["placeholder"].getSize().y);
    battle_drawables_.emplace_back(rightPanel);

    bottomPanel->setTexture(textures_["deck"]);
    bottomPanel->setOrigin(0,textures_["deck"].getSize().y);
    bottomPanel->setPosition(0,window.getSize().y);
    bottomPanel->setColor(sf::Color(250,181,100));
    bottomPanel->setScale((float)window.getSize().x/(float)textures_["deck"].getSize().x, (float)window.getSize().y*0.3/(float)textures_["deck"].getSize().y);
    battle_drawables_.emplace_back(bottomPanel);



    std::sort(battle_drawables_.begin(), battle_drawables_.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});
    for(CustomDrawable* ptr: battle_drawables_){
        ptr->setWindow(&window);
    }
}

void Context::battleground(sf::RenderWindow &window){
    for(CustomDrawable* ptr: battle_drawables_){
        ptr->Draw();
    }
    for(CustomDrawable* ptr: battle_buttons_){
        ptr->Draw();
    }
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    for (Button* btn : battle_buttons_) btn->update(mousePos, isPressed);

}
