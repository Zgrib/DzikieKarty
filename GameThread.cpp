

void Sleep(float seconds){
    sf::sleep(sf::seconds(seconds));
}



/// funkcja ktora bedzie zajmowala sie cala logika gry, osobno od interfejsu
/// w celu kontrolowanych w czasie ruchow kart i innych elementow w swiecie
/// aby cos spowolnic wystarczy dodac Sleep() i wywolac go z tego watku
void GameLoop(sf::RenderWindow* window){
    auto tp = std::chrono::steady_clock::now();





    while(window->isOpen()){

        //the time between frames, not sure how useful it will be?
        float deltaTime;
        {
            const auto _tp = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration<float>(_tp - tp).count();
            tp = _tp;
        }






        if(tempTest != nullptr)
            tempTest->setPosition( tempTest->getPosition().x-7, tempTest->getPosition().y+3 );
        Sleep(0.3);
        //c1->setPosition( (*c1).getPosition().x+4, (*c1).getPosition().y-2 );
        //Sleep(0.2);


    }

}