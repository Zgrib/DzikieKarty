#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <mutex>
#include "Card.h"


class Player{
private:
    std::vector<Card*> deck;

    static Player* instance;
    static std::mutex mtx;
    Player(){}

    ~Player() {
        for (auto* c : deck) delete c;
        deck.clear();
    }


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
            Card* tmp = cloneCard(card);
            cards.emplace_back(*tmp);
            delete tmp;
        }
        return cards;

    }

    void removeCard(Card* c) {
        auto it = std::find(deck.begin(), deck.end(), c);
        if (it != deck.end()) {
            delete *it;
            deck.erase(it);
        }
    }


};












#endif // PLAYER_H
