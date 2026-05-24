
CustomDrawable background(-10);
CustomDrawable leftPanel(-8);
CustomDrawable rightPanel(-8);
CustomDrawable bottomPanel(-8);

void DrawInterface(sf::RenderWindow& window){


    background.setTexture(placeholder);
    background.setColor(sf::Color(200,141,60));
    background.setScale((float)window.getSize().x/(float)placeholder.getSize().x, (float)window.getSize().y/(float)placeholder.getSize().y);
    Drawables.emplace_back(&background);


    leftPanel.setTexture(placeholder);
    leftPanel.setColor(sf::Color(229,161,80));
    leftPanel.setScale((float)window.getSize().x*0.25/(float)placeholder.getSize().x, (float)window.getSize().y*0.7/(float)placeholder.getSize().y);
    Drawables.emplace_back(&leftPanel);



    rightPanel.setTexture(placeholder);
    rightPanel.setPosition(window.getSize().x*0.75,0);
    rightPanel.setColor(sf::Color(229,161,80));
    rightPanel.setScale((float)window.getSize().x*0.25/(float)placeholder.getSize().x, (float)window.getSize().y*0.7/(float)placeholder.getSize().y);
    Drawables.emplace_back(&rightPanel);

    bottomPanel.setTexture(placeholder);
    bottomPanel.setOrigin(0,placeholder.getSize().y);
    bottomPanel.setPosition(0,window.getSize().y);
    bottomPanel.setColor(sf::Color(250,181,100));
    Drawables.emplace_back(&bottomPanel);
    bottomPanel.setScale((float)window.getSize().x/(float)placeholder.getSize().x, (float)window.getSize().y*0.3/(float)placeholder.getSize().y);



}


void LoadTextures(){

    if (!card.loadFromFile("resources/karta.png")) {
        throw (std::string)"Could not load texture !";
    }

    if (!placeholder.loadFromFile("resources/placeholder.png")) {
        throw (std::string)"Could not load texture !";
    }
    if (!cardReverse.loadFromFile("resources/placeholder.png")) {
        throw (std::string)"Could not load texture !";
    }
    if (!cardBackground.loadFromFile("resources/phCard.png")) {
        throw (std::string)"Could not load texture !";
    }
    if (!cardSlot.loadFromFile("resources/phSlot.png")) {
        throw (std::string)"Could not load texture !";
    }


}
