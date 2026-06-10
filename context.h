#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "button.h"

class Context
{
private:
    void load_textures();
    void load_fonts();
    void events_loop(sf::RenderWindow &window, sf::Event &event);
    void start_main_menu(sf::RenderWindow &window);
    void main_menu(sf::RenderWindow &window);
    void window_loop(sf::RenderWindow &window);
    int scene_=0;
    std::map<std::string, sf::Font> fonts_;
    std::vector<Button*> menu_buttons_;
    std::map<std::string, sf::Texture> textures_;
public:
    Context();
    void start_context();
};

#endif // CONTEXT_H
