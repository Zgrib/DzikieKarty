#ifndef LOGTHREAD_H
#define LOGTHREAD_H
//ZMIENIC TEN PLIK NA .CPP
void LogLoop(sf::RenderWindow* window){

    GameLog log = GameLog(window);
    log.setPosition(400,400);
    if(fonts!=nullptr)
        log.text->setFont(fonts->font);
    else{
        sf::sleep(sf::seconds(0.01));
        log.text->setFont(fonts->font);
    }
    Drawables.emplace_back(&log);
    log.setPosition(900,10);
    while(window->isOpen()){


        log.addText("o");
        sf::sleep(sf::seconds(0.05));

    }

}

#endif // LOGTHREAD_H
