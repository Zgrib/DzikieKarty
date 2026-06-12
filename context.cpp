#include "context.h"
#include "button.h"
#include "GameLog.h"
#include "TemplateDeck.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
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
        {"button_pressed",       "resources/Button_pressed.png"},
        {"background",       "resources/background.png"},
        {"background-2",       "resources/background-2.png"},
        {"background-3",       "resources/background-3.png"},
        {"background-4",       "resources/background-4.png"},
        {"background-5",       "resources/background-5.png"},
        {"background-6",       "resources/background-6.png"},
        {"background-7",       "resources/background-7.png"},
        {"background-8",       "resources/background-8.png"}
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

    manager_ = new GameManager(window, this);
    manager_->initVisualSlots(textures_["phSlot"]);


    Context::start_main_menu(window);
    Context::start_battleground(window);
    Context::start_choice_menu(window);
    manager_->getPlayer().initializeDeck(this, window);
    manager_->getPlayer().prepareForBattle(manager_);
    int startIncome = 3;
    manager_->aiDirectorCredits += startIncome;

    manager_->getAI().makeMove(manager_);


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
        // poco kurka wonda wiece po ang pisać opisy, jesteśmy w polsce a i tak ponmiżej piszesz po polsku jeden z drugim
        // i to nie tak że ktoś czegoś nie rozumie bo biegle mówie

        //womp womp
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

        if(scene_== 2){
            Context::choice_menu(window);

        }
        // end the current frame
        window.display();
    }
}

void Context::events_loop(sf::RenderWindow &window, sf::Event &event) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // REAKCJA NA KLIKNIĘCIE MYSZKĄ
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2i mappedMousePos(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));

            if (scene_ == 1) {
                bool isBattleOver = (manager_->getBattleEngine().getCurrentState() == BattleState::BATTLE_OVER);

                bool buttonClicked = false;
                for (Button* btn : battle_buttons_) {
                    if (btn->isVisible() && btn->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        btn->update(mappedMousePos, true);
                        buttonClicked = true;
                        break;
                    }
                }
                if (buttonClicked) {
                    continue;
                }
                if (isBattleOver) {
                    continue;
                }

                // --- 1. OBSŁUGA WYBORU KARTY Z RĘKI ---
                Player* player = manager_->getBattleEngine().player;
                bool handCardClicked = false;
                if (player != nullptr) {
                    for (Card* card : player->hand) {
                        if (card->getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                            if (manager_->canDraw == true) {
                                GameLog::add("Najpierw dobierz karte!");
                                handCardClicked = true;
                                break;
                            }

                            // Zmiana karty - resetujemy podświetlenia starych ofiar i flagę opłacenia
                            if (manager_->selectedCard != nullptr && manager_->selectedCard != card) {
                                manager_->selectedCard->setSelected(false);
                                for (Card* c : manager_->cardsToSacrifice) {
                                    c->setSacrificeHighlight(false);
                                }
                                manager_->cardsToSacrifice.clear();
                                player->updateHandPositions();
                            }

                            manager_->selectedCard = card;
                            manager_->cardsToSacrifice.clear();
                            manager_->bloodCostPaid = false; // Nowa wybrana karta = koszt jeszcze nieopłacony

                            card->setPosition(card->getPosition().x, window.getSize().y - card->getGlobalBounds().height - 70.f);
                            card->setSelected(true);
                            std::cout << "Zaznaczono karte z reki!\n";
                            handCardClicked = true;
                            break;
                        }
                    }
                }

                if (handCardClicked) {
                    continue;
                }

                // --- 2. LOGIKA KOSZTU: KOŚCI (CostType::BONES) ---
                if (manager_->selectedCard != nullptr && manager_->selectedCard->getCost() > 0 && manager_->selectedCard->costType_ == CostType::BONES) {
                    if (manager_->bones < manager_->selectedCard->getCost()) {
                        GameLog::add("Masz za malo kosci! Potrzebujesz: " + std::to_string(manager_->selectedCard->getCost()));

                        manager_->selectedCard->setSelected(false);
                        manager_->selectedCard = nullptr;
                        if (player != nullptr) player->updateHandPositions();
                        continue;
                    }
                }

                // --- 3. LOGIKA KOSZTU: KREW (CostType::BLOOD) ---
                bool sacrificeClicked = false;
                // Logikę odpalamy TYLKO wtedy, gdy koszt NIE został jeszcze w pełni opłacony
                if (manager_->selectedCard != nullptr && manager_->selectedCard->getCost() > 0 &&
                    manager_->selectedCard->costType_ == CostType::BLOOD && !manager_->bloodCostPaid) {

                    int requiredCost = manager_->selectedCard->getCost();

                    for (int c = 0; c < 4; ++c) {
                        Card* cardOnBoard = manager_->getBattleEngine().board[c][1]; // rząd 1 = gracz
                        if (cardOnBoard != nullptr && cardOnBoard->getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                            auto it = std::find(manager_->cardsToSacrifice.begin(), manager_->cardsToSacrifice.end(), cardOnBoard);

                            if (it != manager_->cardsToSacrifice.end()) {
                                cardOnBoard->setSacrificeHighlight(false);
                                manager_->cardsToSacrifice.erase(it);
                                std::cout << "Odznaczono karte z ofiary.\n";
                            } else {
                                cardOnBoard->setSacrificeHighlight(true);
                                manager_->cardsToSacrifice.push_back(cardOnBoard);
                                std::cout << "Zaznaczono ofiare (" << manager_->cardsToSacrifice.size() << "/" << requiredCost << ")\n";

                                if (manager_->cardsToSacrifice.size() == static_cast<size_t>(requiredCost)) {
                                    std::cout << "Osiagnieto wymagany koszt! Karty natychmiastowo znikaja.\n";

                                    for (Card* deadCard : manager_->cardsToSacrifice) {
                                        for (int boardCol = 0; boardCol < 4; ++boardCol) {
                                            if (manager_->getBattleEngine().board[boardCol][1] == deadCard) {
                                                manager_->getBattleEngine().board[boardCol][1] = nullptr;
                                                break;
                                            }
                                        }
                                        manager_->removeDeployedCard(deadCard);
                                        delete deadCard;
                                    }
                                    manager_->cardsToSacrifice.clear();

                                    // ZAMIAST ZMIENIAĆ KOSZT KARTY, USTAWIAMY FLAGĘ:
                                    manager_->bloodCostPaid = true;
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

                // Ścisła blokada: gracz klika na puste pole, karta kosztuje krew, ale flaga bloodCostPaid jest FALSE
                if (manager_->selectedCard != nullptr && manager_->selectedCard->getCost() > 0 &&
                    manager_->selectedCard->costType_ == CostType::BLOOD && !manager_->bloodCostPaid) {
                    GameLog::add("Musisz najpierw poswiecic jednostki!");
                    continue;
                }

                // --- 4. POSTAWIENIE KARTY NA PLANSZY ---
                bool boardClicked = manager_->handleBoardClick(mousePos);
                if (boardClicked) {
                    continue;
                }
            }
        }
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
        background->setTexture(textures_["background-4"]);
        background->setColor(sf::Color(255,255,255));
        background->setScale((float)window.getSize().x/(float)textures_["background-5"].getSize().x, (float)window.getSize().y/(float)textures_["background-5"].getSize().y);
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
void Context::start_choice_menu(sf::RenderWindow &window){

        std::vector<Button*> tmp_btn;
        Button* btn_1=new Button(5,textures_["card"],textures_["card"],fonts_["papyrus"],"WRONA.",&window);
        tmp_btn.emplace_back(btn_1);
        Button* btn_2=new Button(5,textures_["card"],textures_["card"],fonts_["papyrus"],"WILK",&window);
                tmp_btn.emplace_back(btn_2);
        Button* btn_3=new Button(5,textures_["card"],textures_["card"],fonts_["papyrus"],"WEZ",&window);
                tmp_btn.emplace_back(btn_3);
        Button* btn_4=new Button(5,textures_["card"],textures_["card"],fonts_["papyrus"],"NIETOPERZ",&window);
                tmp_btn.emplace_back(btn_4);
        Button* btn_5=new Button(5,textures_["card"],textures_["card"],fonts_["papyrus"],"WIEWIORKA",&window);
                tmp_btn.emplace_back(btn_5);
                // ustaw akcje dla przycisków — każdy przycisk tworzy odpowiednią kartę bezpośrednio
                tmp_btn[0]->setOnClickAction([this, &window](){ // WRONA
                    Player& p = manager_->getPlayer();
                    TemplateDeck td; CardStats s = td.generateCardStats(CreatureType::WRONA);
                    Card* c = manager_->BuildCard(s, textures_["raven"], textures_["card"], fonts_["papyrus"], window, 0);
                    p.addCard(c); scene_ = 1;
                });
                tmp_btn[1]->setOnClickAction([this, &window](){ // WILK
                    Player& p = manager_->getPlayer();
                    TemplateDeck td; CardStats s = td.generateCardStats(CreatureType::WILK);
                    Card* c = manager_->BuildCard(s, textures_["wilk"], textures_["card"], fonts_["papyrus"], window, 0);
                    p.addCard(c); scene_ = 1;
                });
                tmp_btn[2]->setOnClickAction([this, &window](){ // WEZ
                    Player& p = manager_->getPlayer();
                    TemplateDeck td; CardStats s = td.generateCardStats(CreatureType::WEZ);
                    Card* c = manager_->BuildCard(s, textures_["wez"], textures_["card"], fonts_["papyrus"], window, 0);
                    p.addCard(c); scene_ = 1;
                });
                tmp_btn[3]->setOnClickAction([this, &window](){ // NIETOPERZ
                    Player& p = manager_->getPlayer();
                    TemplateDeck td; CardStats s = td.generateCardStats(CreatureType::NIETOPERZ);
                    Card* c = manager_->BuildCard(s, textures_["nietoperz"], textures_["card"], fonts_["papyrus"], window, 0);
                    p.addCard(c); scene_ = 1;
                });
                tmp_btn[4]->setOnClickAction([this, &window](){ // WIEWIORKA
                    Player& p = manager_->getPlayer();
                    TemplateDeck td; CardStats s = td.generateCardStats(CreatureType::WIEWIORKA);
                    Card* c = manager_->BuildCard(s, textures_["wiewiorka"], textures_["card"], fonts_["papyrus"], window, 0);
                    p.addCard(c); scene_ = 1;
                });
                                srand(time(0));
                int randomNum = rand() %5;
        tmp_btn[randomNum]->setPosition(300,400);
        choice_buttons_.emplace_back(tmp_btn[randomNum]);

    randomNum = rand() %5;
tmp_btn[randomNum]->setPosition(1300,400);
choice_buttons_.emplace_back(tmp_btn[randomNum]);
        CustomDrawable* background= new CustomDrawable(-10);
        background->setTexture(textures_["background-2"]);
        background->setColor(sf::Color(255,255,255));
        background->setScale((float)window.getSize().x/(float)textures_["background-5"].getSize().x, (float)window.getSize().y/(float)textures_["background-5"].getSize().y);
        choice_drawables_.emplace_back(background);



        std::sort(choice_drawables_.begin(), choice_drawables_.end(), [](  CustomDrawable* a,   CustomDrawable* b){ return (a->getZ() < b->getZ());});
        for(CustomDrawable* ptr: choice_drawables_){
            ptr->setWindow(&window);
        }
}
void Context::choice_menu(sf::RenderWindow &window){

    for(CustomDrawable* ptr: choice_drawables_){
        ptr->Draw();
    }

    for(Button* ptr: choice_buttons_){
        ptr->Draw();
    }
    // in window_loop (each frame)
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2i mappedMousePos(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
    for (Button* btn : choice_buttons_) btn->update(mappedMousePos, isPressed);

}

void Context::start_battleground(sf::RenderWindow &window){
    CustomDrawable* background= new CustomDrawable(-10);
    CustomDrawable* leftPanel= new CustomDrawable(-8);
    CustomDrawable* rightPanel= new CustomDrawable(-8);
    CustomDrawable* bottomPanel = new CustomDrawable(-8);

    CustomTextDrawable* szalaPlayer = new CustomTextDrawable(&(manager_->getPlayer().damageTaken),2);
    CustomTextDrawable* szalaAI = new CustomTextDrawable(&(manager_->getAI().damageTaken),2);

    CustomTextDrawable* szalaPlayerD = new CustomTextDrawable(2);
    CustomTextDrawable* szalaAID = new CustomTextDrawable(2);
    CustomTextDrawable* bones = new CustomTextDrawable(&(manager_->bones),2);

    bones->text->setFont(fonts_["papyrus"]);
    bones->text->setCharacterSize(24);
    bones->before="Bone count: ";
    bones->text->setPosition(50.f, 100.f);
    battle_drawables_.emplace_back(bones);



    szalaPlayerD->text->setFont(fonts_["papyrus"]);
    szalaPlayerD->text->setCharacterSize(24);
    szalaPlayerD->text->setString("Player damage taken\n" );
    szalaPlayerD->text->setPosition(0.f, 500.f);
    battle_drawables_.emplace_back(szalaPlayerD);



    szalaAID->text->setFont(fonts_["papyrus"]);
    szalaAID->text->setCharacterSize(24);
    szalaAID->text->setString("Enemy damage taken\n") ;
    szalaAID->text->setPosition(250.f, 500.f);
    battle_drawables_.emplace_back(szalaAID);

    szalaPlayer->text->setFont(fonts_["papyrus"]);
    szalaPlayer->text->setCharacterSize(50);
    szalaPlayer->text->setPosition(50.f, 400.f);
    szalaPlayer->text->setFillColor(sf::Color::White);
    battle_drawables_.emplace_back(szalaPlayer);


    szalaAI->text->setFont(fonts_["papyrus"]);
    szalaAI->text->setCharacterSize(50);
    szalaAI->text->setFillColor(sf::Color::White);
    szalaAI->text->setPosition(400.f, 400.f);
    battle_drawables_.emplace_back(szalaAI);



    std::cout << "Test/n";

    background->setTexture(textures_["background-7"]);
    //background->setColor(sf::Color(200,141,60));
    background->setPosition((float)window.getSize().x*0.25,0);
    background->setScale((float)window.getSize().x*0.5/(float)textures_["background-5"].getSize().x, (float)window.getSize().y*0.7/(float)textures_["background-5"].getSize().y);
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

    // tworzymy gamelog i wrzucamy do drawables bitwy
    GameLog* logger = new GameLog(fonts_["papyrus"], &window);
    battle_drawables_.emplace_back(logger);

    // testowy log na start
    GameLog::add("Bitwa rozpoczeta!");
    GameLog::add("Powodzenia.");


    bottomPanel->setTexture(textures_["background"]);
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


    // //delete this button later
    // Button* quit = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"WYJDZ",&window);
    // quit->setPosition(1800,50);
    // quit->setOnClickAction([this](){scene_=0;});//and maybe do other stuff?
    // battle_buttons_.emplace_back(quit);


    Button* endTurn = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"Zakoncz ture",&window);
    endTurn->setPosition(200,700);
    endTurn->setOnClickAction([this, &window](){
        if(manager_->canDraw == true){
            GameLog::add("Najpierw dobierz karte!");
        }
        else {
            // Czyszczenie zaznaczeń gracza przy końcu tury
            if (manager_->selectedCard != nullptr) {
                manager_->selectedCard->setSelected(false);
                manager_->selectedCard = nullptr;
                for (Card* c : manager_->cardsToSacrifice) {
                    c->setSacrificeHighlight(false);
                }
                manager_->cardsToSacrifice.clear();
                Player* player = manager_->getBattleEngine().player;
                if (player != nullptr) {
                    player->updateHandPositions();
                }
            }

            std::cout << "Koniec tury! Odpalam silnik...\n";
            GameLog::add("-> koniec tury");
            int turnIncome = 3;
            manager_->aiDirectorCredits += turnIncome;
            manager_->getBattleEngine().EndTurn();
        }

    });
    battle_buttons_.emplace_back(endTurn);


    // PRZYCISK 1: Dobieranie z Talii
    Button* drawCardBtn = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"Dobierz karte",&window);
    drawCardBtn->setPosition(50.f, 960.f);
    drawCardBtn->setOnClickAction([this]() {
        if(manager_->canDraw==true){
            // Dobieramy kartę z talii bitewnej
            manager_->getPlayer().drawCard();
            GameLog::add("-> karta zostala dobrana");

            manager_->canDraw=false;
        }
        else{
            GameLog::add("-> Nie mozesz teraz dobrac karty");
        }

    });
    battle_buttons_.emplace_back(drawCardBtn);


    endBattleButton_ = new Button(5, textures_["button"], textures_["button_pressed"], fonts_["papyrus"], "Zakoncz bitwe", &window);
    endBattleButton_->setPosition(1920/2.f -50, 100.f);
    endBattleButton_->setScale(1.2,1.2);
    endBattleButton_->setOnClickAction([this]() {
        scene_ = 2;
    });
    endBattleButton_->setVisible(false);
    battle_buttons_.emplace_back(endBattleButton_);


    Button* squirrelBtn = new Button(5,textures_["button"],textures_["button_pressed"],fonts_["papyrus"],"Dobierz wiewiorke",&window);
    squirrelBtn->setPosition(320.f, 960.f);
    squirrelBtn->setOnClickAction([this, &window]() {
        if(manager_->canDraw==true){
            TemplateDeck tmp_deck;
            CardStats squirrelStats = tmp_deck.generateCardStats(CreatureType::WIEWIORKA);

            Card* squirrel = manager_->BuildCard(
                squirrelStats,
                textures_["squirel"],
                textures_["card"],
                fonts_["papyrus"],
                window,
                0
                );

            manager_->canDraw=false;
            manager_->getPlayer().drawSquirrel(squirrel);
            GameLog::add("-> Wiewiorka zostala dobrana");
        }
        else{
            GameLog::add("-> Nie mozesz teraz dobrac karty");
        }
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
    bool isBattleOver = (manager_->getBattleEngine().getCurrentState() == BattleState::BATTLE_OVER);


    for (Button* btn : battle_buttons_) {
        if (isBattleOver) {

            if (btn == endBattleButton_) {
                btn->setVisible(true);
            } else {
                btn->setVisible(false);
            }
        } else {

            if (btn == endBattleButton_) {
                btn->setVisible(false);
            } else {
                btn->setVisible(true);
            }
        }
    }


    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2i mappedMousePos(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
    for (Button* btn : battle_buttons_) btn->update(mappedMousePos, isPressed);

    for(Button* ptr: battle_buttons_){
        if(ptr->isVisible())
            ptr->Draw();
        else continue;
    }


}



/*
 *
 * funkcja sprzed dodania kosci
void Context::events_loop(sf::RenderWindow &window, sf::Event &event) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // REAKCJA NA KLIKNIĘCIE MYSZKĄ
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2i mappedMousePos(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));

            if (scene_ == 1) {
                bool isBattleOver = (manager_->getBattleEngine().getCurrentState() == BattleState::BATTLE_OVER);

                bool buttonClicked = false;
                for (Button* btn : battle_buttons_) {
                    // Sprawdzamy TYLKO widoczne przyciski i czy mysz się na nich znajduje
                    if (btn->isVisible() && btn->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        btn->update(mappedMousePos, true);
                        buttonClicked = true;
                        break; // Znaleziono kliknięty przycisk, wychodzimy z pętli for
                    }
                }
                if (buttonClicked) {
                    continue;
                }
                if (isBattleOver) {
                    continue;
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

                                    for (Card* deadCard : manager_->cardsToSacrifice) {
                                        for (int boardCol = 0; boardCol < 4; ++boardCol) {
                                            if (manager_->getBattleEngine().board[boardCol][1] == deadCard) {
                                                manager_->getBattleEngine().board[boardCol][1] = nullptr;
                                                break;
                                            }
                                        }
                                        manager_->removeDeployedCard(deadCard);
                                        delete deadCard;
                                    }
                                    manager_->cardsToSacrifice.clear();
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

                // Obsługa wyboru karty z ręki
                Player* player = manager_->getBattleEngine().player;
                if (player != nullptr) {
                    for (Card* card : player->hand) {
                        if (card->getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                            if (manager_->canDraw == true) {
                                GameLog::add("Najpierw dobierz karte!");
                                continue;
                            }

                            if (manager_->selectedCard != nullptr && manager_->selectedCard != card) {
                                manager_->selectedCard->setSelected(false);
                                player->updateHandPositions();
                            }

                            manager_->selectedCard = card;
                            manager_->cardsToSacrifice.clear();

                            card->setPosition(card->getPosition().x, window.getSize().y - card->getGlobalBounds().height - 70.f);
                            card->setSelected(true);
                            std::cout << "Zaznaczono karte z reki!\n";
                            break;
                        }
                    }
                }
            } // scene_ == 1
        }
    }
}*/
