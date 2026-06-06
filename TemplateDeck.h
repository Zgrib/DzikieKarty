#pragma once
#include <unordered_map>
#include <string>
#include <random>
#include <memory>
#include "Card.h" // BuildCard, Card, CostType

enum class CreatureType { WILK, NIETOPERZ, WRONA, WEZ, WIEWIORKA };

class TemplateDeck {
public:
    // Konstruktor przyjmuje mapę tekstur (klucz np. "wilk")
    TemplateDeck(const std::unordered_map<std::string, sf::Texture>& textures)
        : textures(textures), rng(std::random_device{}())
    {
        // zdefiniuj domyślne costType dla każdego wzorca (możesz zmienić)
        templates = {
            {CreatureType::WILK,      {"wilk",      CostType::BONES}},
            {CreatureType::NIETOPERZ, {"nietoperz", CostType::BLOOD}},
            {CreatureType::WRONA,     {"wrona",     CostType::BONES}},
            {CreatureType::WEZ,       {"wez",       CostType::BLOOD}},
            {CreatureType::WIEWIORKA, {"wiewiorka", CostType::BONES}}
        };
    }

    // difficulty: 1 (easy), 2 (normal), 3 (hard)
    // zwraca nowy Card* stworzony przez BuildCard (alokacja dynamiczna - caller odpowiada za delete lub użycie smart ptr)
    Card* getRandomCard(CreatureType type, int difficulty = 2, int z = 0){
        difficulty = clampDifficulty(difficulty);
        auto tplIt = templates.find(type);
        if(tplIt == templates.end()) return nullptr;

        const TemplateInfo& info = tplIt->second;
        sf::Texture tex;
        auto texIt = textures.find(info.textureKey);
        if(texIt != textures.end()) tex = texIt->second;
        else {
            // jeśli brak tekstury, użyj pustej tekstury (BuildCard radzi sobie)
        }

        // zakres bazowy 1..10, modyfikowany przez difficulty: easy -> niższe średnie, hard -> wyższe
        // Użyjemy prostego przesunięcia: easy: -3, normal: 0, hard: +3 (klampujemy do [1,10])
        int bias = (difficulty == 1) ? -3 : (difficulty == 3) ? 3 : 0;

        int health = clampStat(randInRange(1,10) + bias);
        int damage = clampStat(randInRange(1,10) + bias);
        int cost   = clampStat(randInRange(1,10) + (bias > 0 ? 0 : 0)); // cost nie przesuwamy mocno; tu neutralnie

        // dla różnorodności można dopasować typowe staty per gatunek (np. wilk wyższy dmg)
        applySpeciesModifiers(type, health, damage, cost);

        return BuildCard(health, damage, cost, info.costType, tex, z);
    }

private:
    struct TemplateInfo {
        std::string textureKey;
        CostType costType;
    };

    std::unordered_map<CreatureType, TemplateInfo> templates;
    const std::unordered_map<std::string, sf::Texture>& textures;
    std::mt19937 rng;

    int randInRange(int a, int b){
        std::uniform_int_distribution<int> dist(a,b);
        return dist(rng);
    }

    int clampStat(int v){
        if(v < 1) return 1;
        if(v > 10) return 10;
        return v;
    }

    int clampDifficulty(int d){
        if(d < 1) return 1;
        if(d > 3) return 3;
        return d;
    }

    void applySpeciesModifiers(CreatureType type, int& health, int& damage, int& cost){
        // lekkie, przykładowe modyfikatory (dostosuj wg potrzeby)
        switch(type){
            case CreatureType::WILK:
                damage = clampStat(damage + 2);
                health = clampStat(health + 1);
                cost = std::max(1, cost);
                break;
            case CreatureType::NIETOPERZ:
                damage = clampStat(damage - 1);
                health = clampStat(health - 1);
                cost = std::max(1, cost-1);
                break;
            case CreatureType::WRONA:
                damage = clampStat(damage);
                health = clampStat(health - 1);
                cost = std::max(1, cost-1);
                break;
            case CreatureType::WEZ:
                damage = clampStat(damage + 1);
                health = clampStat(health - 1);
                break;
            case CreatureType::WIEWIORKA:
                damage = clampStat(damage - 2);
                health = clampStat(health - 2);
                cost = std::max(1, cost-1);
                break;
        }
    }
};
