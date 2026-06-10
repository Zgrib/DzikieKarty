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
        {"Battleground","resources/Battleground.png"}
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

sf::RenderWindow window(sf::VideoMode(1920, 1800), "Dzikie Karty");
window.setFramerateLimit(60);
Context::start_main_menu(window);
Context::window_loop(window);

}

void Context::window_loop(sf::RenderWindow &window){
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        Context::events_loop(window,event);

        // clear the window with black color
        window.clear(sf::Color::Red);

        // draw everything here...
            if(scene_== 0){
            Context::main_menu(window);
}

        // end the current frame
        window.display();
    }
}

void Context::events_loop(sf::RenderWindow &window,sf::Event &event){
    while (window.pollEvent(event)) {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
//        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
//            window.close();
//        for(Button* ptr: menu_buttons_){
//            ptr->update(sf::Vector2i(0,0),false);
//        }
//                    if (event.type == sf::Event::MouseButtonPressed) {
//                        if(event.mouseButton.button == sf::Mouse::Left) {
//                            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
//                            for(Button* ptr: menu_buttons_){
//                                ptr->update(mouse_pos,true);
//                            }
//                        }

                        //        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                        //        bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                        //        Button1->update(currentMousePos, isLeftPressed);
                        //        // TEST PRZYCISKU
//    }
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

}
void Context::main_menu(sf::RenderWindow &window){
for(Button* ptr: menu_buttons_){
    ptr->Draw();
}
// in window_loop (each frame)
sf::Vector2i mousePos = sf::Mouse::getPosition(window);
bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
for (Button* btn : menu_buttons_) btn->update(mousePos, isPressed);

}
