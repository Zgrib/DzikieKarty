#include "GameLog.h"
//#include <sstream>

GameLog* GameLog::instance = nullptr;

GameLog::GameLog(const sf::Font& font, sf::RenderWindow* window) : CustomDrawable(100) {
    this->window = window;
    instance = this;

    // napis gamelog obok przycisku
    title.setFont(font);
    title.setString("Gamelog");
    title.setCharacterSize(35);
    title.setFillColor(sf::Color::Black);
    title.setOutlineThickness(0.5);
    title.setPosition(1500, 30);

    // miejsce na wlasciwe logi
    logText.setFont(font);
    logText.setCharacterSize(20);
    logText.setOutlineThickness(0.5);
    logText.setFillColor(sf::Color::Black);
    logText.setPosition(1500, 110);
}
void GameLog::clear(){
    for(int i=0; i<17;i++)
        GameLog::add(" ");

}

void GameLog::add(const std::string& msg) {
    if (!instance) return;

    std::string currentLine = "";
    float maxWidth = 420.f;

    for (size_t i = 0; i < msg.length(); ++i) {
        currentLine += msg[i];
        instance->logText.setString(currentLine);

        if (instance->logText.getLocalBounds().width > maxWidth) {
            size_t lastSpace = currentLine.find_last_of(' ');

            if (lastSpace != std::string::npos && lastSpace > 0) {
                instance->messages.push_back(currentLine.substr(0, lastSpace));
                currentLine = currentLine.substr(lastSpace + 1);
            } else {
                instance->messages.push_back(currentLine.substr(0, currentLine.length() - 1));
                currentLine = msg[i];
            }
        }
    }

    if (!currentLine.empty()) {
        instance->messages.push_back(currentLine);
    }

    while (instance->messages.size() > instance->maxLines) {
        instance->messages.pop_front();
    }

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