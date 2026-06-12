#include "GameLog.h"

GameLog* GameLog::instance = nullptr;

GameLog::GameLog(const sf::Font& font, sf::RenderWindow* window) : CustomDrawable(100) {
    this->window = window;
    instance = this;

    // napis gamelog obok przycisku
    title.setFont(font);
    title.setString("gamelog");
    title.setCharacterSize(35);
    title.setFillColor(sf::Color::Black);
    title.setPosition(1500, 30);

    // miejsce na wlasciwe logi
    logText.setFont(font);
    logText.setCharacterSize(25);
    logText.setFillColor(sf::Color::Black);
    logText.setPosition(1500, 110);
}

void GameLog::add(const std::string& msg) {
    if (!instance) return; // jak obiekt nie istnieje to zlewamy

    instance->messages.push_back(msg);

    // jak wyjezdza poza limit to wywalamy pierwszy z gory
    if (instance->messages.size() > instance->maxLines) {
        instance->messages.pop_front();
    }

    // sklejamy cala kolejke w jeden wielki string z enterami
    std::string full = "";
    for (const auto& m : instance->messages) {
        full += m + "\n";
    }
    instance->logText.setString(full);
}

void GameLog::Draw() {
    if (window) {
        window->draw(title);
        window->draw(logText);
    }
}