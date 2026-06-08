#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "BattleEngine.cpp"



void Sleep(float seconds){
    sf::sleep(sf::seconds(seconds));
}

sf::Vector2f lerp(sf::Vector2f start, sf::Vector2f end, float t) {
    return start + t * (end - start);
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



        //do some bulllshiiiit


        //PrepareBattle();
        Sleep(TIMESTEP*3);





    }

}