#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include "Card.h" // zakładam że zawiera definicję Card i Interactive

enum class Owner { Player, Enemy };

class Deck {
public:
    Deck(float startX = 50.f, float startYPlayer = 400.f, float startYEnemy = 50.f, float spacing = 110.f)
        : startX(startX), startYPlayer(startYPlayer), startYEnemy(startYEnemy), spacing(spacing)
    {}

    ~Deck(){
        // nie usuwamy kart tu agresywnie jeżeli są używane gdzie indziej;
        // jeżeli Deck jest właścicielem, usuń tutaj.
    }

    // Dodaj kartę do wskazanego decku
    void addCard(Card* card, Owner owner){
        if(owner == Owner::Player) playerDeck.push_back(card);
        else enemyDeck.push_back(card);
        updateLayout(owner);
    }

    // Tasuj wybrany deck
    void shuffle(Owner owner){
        auto& d = (owner == Owner::Player) ? playerDeck : enemyDeck;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(d.begin(), d.end(), g);
        updateLayout(owner);
    }

    // Dobierz i zwróć kartę z góry (nullptr jeśli puste)
    Card* drawTop(Owner owner){
        auto& d = (owner == Owner::Player) ? playerDeck : enemyDeck;
        if(d.empty()) return nullptr;
        Card* c = d.front();
        d.erase(d.begin());
        updateLayout(owner);
        return c;
    }

    // Liczba kart
    size_t size(Owner owner) const {
        return (owner == Owner::Player) ? playerDeck.size() : enemyDeck.size();
    }

    // Render wszystkich kart (najpierw enemy potem player żeby player był na wierzchu)
    void render(sf::RenderWindow& window){
        for(auto* c : enemyDeck){
            c->Draw();
        }
        for(auto* c : playerDeck){ c->Draw();}
    }

    // Obsługa zdarzenia myszy: klik = toggle selected; pod uwagę bierzemy tylko kliknięcia LMB
    // window potrzebne do pobrania pozycji myszy względem okna
    void handleEvent(const sf::Event& event, sf::RenderWindow& window){
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // sprawdź player deck od końca (top) do początku, aby kliknięcie trafiało w kartę na wierzchu
            for(auto it = playerDeck.rbegin(); it != playerDeck.rend(); ++it){
                Card* c = *it;
                if(!c) continue;
                if(c->contains(mousePos)){
                    bool newSel = !c->isSelected();
                    c->setSelected(newSel);
                    // opcjonalnie: zmień kolor natywnie przez Interactive API
                    if(newSel) c->setColor(sf::Color(230,230,50));
                    else c->setColor(c->getOrginalColor());
                    break; // jedna karta na klik
                }
            }
            // podobnie można włączyć obsługę decku enemy jeśli potrzebne
        }
    }

    // Pobierz wektor wybranych kart (player)
    std::vector<Card*> getSelectedCards() const{
        std::vector<Card*> out;
        for(auto* c : playerDeck) if(c && c->isSelected()) out.push_back(c);
        return out;
    }

    // Wyczyść selection dla wszystkich kart gracza
    void clearSelection(){
        for(auto* c : playerDeck){
            if(c && c->isSelected()){
                c->setSelected(false);
                c->setColor(c->getOrginalColor());
            }
        }
    }

    // Usuń wszystkie karty (niezwalniające pamięci — zależy od zarządzania pamięcią w projekcie)
    void clear(Owner owner){
        if(owner == Owner::Player) playerDeck.clear();
        else enemyDeck.clear();
    }

    // Ustawienia layoutu — pozycjonuje karty poziomo od startX
    void updateLayout(Owner owner){
        auto& d = (owner == Owner::Player) ? playerDeck : enemyDeck;
        float y = (owner == Owner::Player) ? startYPlayer : startYEnemy;
        // centrowanie: możemy wyśrodkować względem startX + połowa szerokości puli
        for(size_t i=0;i<d.size();++i){
            Card* c = d[i];
            if(!c) continue;
            float x = startX + i * spacing;
            c->setPosition(sf::Vector2f(x, y));
        }
    }

    // Przydatne: zainicjuj layout dla obu decków
    void updateAllLayouts(){
        updateLayout(Owner::Player);
        updateLayout(Owner::Enemy);
    }

private:
    std::vector<Card*> playerDeck;
    std::vector<Card*> enemyDeck;
    float startX;
    float startYPlayer;
    float startYEnemy;
    float spacing;
};
