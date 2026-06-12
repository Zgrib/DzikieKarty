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
        {"deck",       "resources/Deck.png"},
        {"chat",       "resources/chat.png"},
        {"waga",       "resources/waga.png"},
        {"button",       "resources/Button.png"},
        {"button_pressed",       "resources/Button_pressed.png"}
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
    manager_->initVisualSlots(textures_["phSlot"]);


    Context::start_main_menu(window);
    Context::start_battleground(window);

    manager_->getBattleEngine().player->initializeDeck(this, window);
    manager_->getBattleEngine().player->prepareForBattle(manager_);
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

            size_t cardsLeft = manager_->getBattleEngine().player->battleDeck.size();
            manager_->cardCounter->text->setString("Talia: " + std::to_string(cardsLeft) + " kart");
        }


        // end the current frame
        window.display();
    }
}

void Context::events_loop(sf::RenderWindow &window,sf::Event &event){
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed){
            window.close();
        }

        // REAKCJA NA KLIKNIĘCIE MYSZKĄ
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (scene_ == 1) {
                // 1. Sprawdzamy przyciski bitwy (Wyjdź, Dobierz itp.)
                for (Button* btn : battle_buttons_) {
                    // Zakładam, że Twoja klasa Button ma mechanizm update/kliknięcia podobny do menu
                    // btn->update(sf::Mouse::getPosition(window), true);
                }
                bool sacrificeClicked = false;
                if (manager_->selectedCard != nullptr && manager_->selectedCard->getCost() > 0) {
                    int requiredCost = manager_->selectedCard->getCost();

                    for (int c = 0; c < 4; ++c) {
                        Card* cardOnBoard = manager_->getBattleEngine().board[c][1]; // rząd 1 = gracz
                        if (cardOnBoard != nullptr && cardOnBoard->getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                            auto it = std::find(manager_->cardsToSacrifice.begin(), manager_->cardsToSacrifice.end(), cardOnBoard);

                            if (it != manager_->cardsToSacrifice.end()) {
                                // Odznaczenie karty
                                cardOnBoard->setSacrificeHighlight(false);
                                manager_->cardsToSacrifice.erase(it);
                                std::cout << "Odznaczono karte z ofiary.\n";
                            } else {
                                // Zaznaczenie karty
                                cardOnBoard->setSacrificeHighlight(true);
                                manager_->cardsToSacrifice.push_back(cardOnBoard);
                                std::cout << "Zaznaczono ofiare (" << manager_->cardsToSacrifice.size() << "/" << requiredCost << ")\n";

                                // SPRAWDZAMY CZY MAMY JUŻ KOMPLET OFIAR
                                if (manager_->cardsToSacrifice.size() == static_cast<size_t>(requiredCost)) {
                                    std::cout << "Osiagnieto wymagany koszt! Karty natychmiastowo znikaja.\n";

                                    // Usuwamy fizycznie i logicznie wszystkie zaznaczone karty
                                    for (Card* deadCard : manager_->cardsToSacrifice) {
                                        // 1. Szukamy jej pozycji w silniku bitwy i usuwamy wskaźnik
                                        for (int boardCol = 0; boardCol < 4; ++boardCol) {
                                            if (manager_->getBattleEngine().board[boardCol][1] == deadCard) {
                                                manager_->getBattleEngine().board[boardCol][1] = nullptr;
                                                break;
                                            }
                                        }
                                        // 2. Usuwamy z wektora rysowania GameManager
                                        manager_->removeDeployedCard(deadCard);

                                        // 3. Zwalniamy pamięć
                                        delete deadCard;
                                    }

                                    // Zostawiamy listę pustą, ale możemy ustawić flagę w managerze, że koszt został opłacony!
                                    manager_->cardsToSacrifice.clear();
                                    // Opcjonalnie: manager_->isCostPaid = true; (jeśli chcesz zablokować ponowne sprawdzanie kosztu)
                                }
                            }
                            sacrificeClicked = true;
                            break;
                        }
                    }
                }

                if (sacrificeClicked) {
                    continue;
                }


                bool boardClicked = manager_->handleBoardClick(mousePos);
                if (boardClicked) {
                    continue;
                }


                // 2. Obsługa wyboru karty z ręki
                Player* player = manager_->getBattleEngine().player;
                if (player != nullptr) {
                    for (Card* card : player->hand) {
                        if (card->getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                            // Jeśli klikałeś już inną kartę, przywróć ją na dół
                            if (manager_->selectedCard != nullptr && manager_->selectedCard != card) {
                                manager_->selectedCard->setSelected(false);
                                player->updateHandPositions(); // Resetuje pozycje pionowe
                            }

                            manager_->selectedCard = card;
                            manager_->cardsToSacrifice.clear();

                            // Unosimy kartę (tylko raz przy kliknięciu)
                            card->setPosition(card->getPosition().x, window.getSize().y - card->getGlobalBounds().height - 70.f);
                            card->setSelected(true);
                            std::cout << "Zaznaczono karte z reki!\n";
                            break;
                        }
                    }
                }
            }
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
        Button* btn_start=new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"START",&window);
        btn_start->setPosition(200,200);
        btn_start->setOnClickAction([this](){scene_=1;});
        menu_buttons_.emplace_back(btn_start);


        Button* btn_quit=new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"WYJDZ",&window);
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
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2i mappedMousePos(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
    for (Button* btn : menu_buttons_) btn->update(mappedMousePos, isPressed);

}


void Context::start_battleground(sf::RenderWindow &window){
    CustomDrawable* background= new CustomDrawable(-10);
    CustomDrawable* leftPanel= new CustomDrawable(-8);
    CustomDrawable* rightPanel= new CustomDrawable(-8);
    CustomDrawable* bottomPanel = new CustomDrawable(-8);

    Button* quit = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"WYJDZ",&window);
    quit->setPosition(1800,50);
    quit->setOnClickAction([this](){scene_=0;});//and maybe do other stuff?
    battle_buttons_.emplace_back(quit);
    std::cout<<"test test test\n";

    Button* endTurn = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"Zakoncz ture",&window);
    endTurn->setPosition(200,700);
    endTurn->setOnClickAction([this](){
        std::cout << "Koniec tury! Odpalam silnik...\n";
        manager_->getBattleEngine().EndTurn();
    });
    battle_buttons_.emplace_back(endTurn);
    std::cout << "Test/n";

    background->setTexture(textures_["Battleground"]);
    //background->setColor(sf::Color(200,141,60));
    background->setPosition((float)window.getSize().x*0.25,0);
    background->setScale((float)window.getSize().x*0.5/(float)textures_["Battleground"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["Battleground"].getSize().y);
    battle_drawables_.emplace_back(background);



    leftPanel->setTexture(textures_["waga"]);
    //leftPanel->setColor(sf::Color(229,161,80));
    leftPanel->setScale((float)window.getSize().x*0.25/(float)textures_["waga"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["waga"].getSize().y);
    battle_drawables_.emplace_back(leftPanel);



    rightPanel->setTexture(textures_["chat"]);
    rightPanel->setPosition(window.getSize().x*0.75,0);
    //rightPanel->setColor(sf::Color(229,161,80));
    rightPanel->setScale((float)window.getSize().x*0.25/(float)textures_["chat"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["chat"].getSize().y);
    battle_drawables_.emplace_back(rightPanel);

    bottomPanel->setTexture(textures_["deck"]);
    bottomPanel->setOrigin(0,textures_["deck"].getSize().y);
    bottomPanel->setPosition(0,window.getSize().y);
    //bottomPanel->setColor(sf::Color(250,181,100));
    bottomPanel->setScale((float)window.getSize().x/(float)textures_["deck"].getSize().x, (float)window.getSize().y*0.3/(float)textures_["deck"].getSize().y);
    battle_drawables_.emplace_back(bottomPanel);

    CustomTextDrawable* deckCounter = new CustomTextDrawable();
    deckCounter->text->setFont(fonts_["papyrus"]);
    deckCounter->text->setCharacterSize(30);
    deckCounter->text->setFillColor(sf::Color::White);
    deckCounter->text->setPosition(50.f, 900.f); // Pozycja w lewym dolnym rogu ekranu
    battle_drawables_.emplace_back(deckCounter);
    manager_->cardCounter = deckCounter;


    // PRZYCISK 1: Dobieranie z Talii
    Button* drawCardBtn = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"Dobierz karte",&window);
    drawCardBtn->setPosition(50.f, 960.f);
    drawCardBtn->setOnClickAction([this]() {
        // Dobieramy kartę z talii bitewnej
        manager_->getBattleEngine().player->drawCard();
    });
    battle_buttons_.emplace_back(drawCardBtn);



    Button* squirrelBtn = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"Dobierz wiewiorke",&window);
    squirrelBtn->setPosition(320.f, 960.f);
    squirrelBtn->setOnClickAction([this, &window]() {
        // Generujemy kartę przez GameManager, aby miała wszystkie teksty, ramki i przypisane okno!
        Card* squirrel = manager_->BuildCard(
            0,                 // Atak
            1,                 // HP
            0,                 // Koszt
            CostType::BLOOD,
            textures_["squirel"], // Poprawiłem literówkę z Twojego load_textures ("squirel")
            textures_["card"],
            fonts_["papyrus"],
            window,
            0
            );

        manager_->getBattleEngine().player->drawSquirrel(squirrel);

        std::cout << "Wygenerowano w pelni funkcjonalna Wiewiorke na rece!\n";
    });
    battle_buttons_.emplace_back(squirrelBtn);

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
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2i mappedMousePos(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
    for (Button* btn : battle_buttons_) btn->update(mappedMousePos, isPressed);

}
