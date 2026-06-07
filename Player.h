#ifndef PLAYER_H
#define PLAYER_H


class Player{
private:
    std::vector<Card*> deck;

    static Player* instance;
    static std::mutex mtx;
    Player(){}

public:
    static Player* getInstance(){
        if(instance==nullptr){
            std::lock_guard<std::mutex> lock(mtx);
            if(instance==nullptr)
                instance = new Player();
        }

        return instance;
    }

    void addCard(const Card* c){
        deck.emplace_back(cloneCard(c));
    }


    std::vector<Card> cloneDeck(){
        //bedzie uzywane na poczatku bitwy do stworzenia "roboczego" decku ktory
        //bedziemy mogli tasowac, usuwac z niego karty itd...
        std::vector<Card> cards;
        for(const auto card:deck){
            cards.emplace_back(*cloneCard(card));
        }
        return cards;

    }

    void removeCard(Card* c){
        //...
    }


};












#endif // PLAYER_H
