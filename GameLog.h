#ifndef GAMELOG_H
#define GAMELOG_H

class GameLog: public CustomTextDrawable{


public:
    GameLog(sf::RenderWindow* _window){
        window = _window;
    }

    void addText(const std::string& s){
        text->setString(text->getString()+s);
    }


};




#endif // GAMELOG_H
