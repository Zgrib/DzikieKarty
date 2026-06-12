#pragma once
#include <unordered_map>
#include <string>
#include "Card.h"
#include <iostream>

enum class CreatureType { WILK, NIETOPERZ, WRONA, WEZ, WIEWIORKA, OPOS, KARALUCH };

class TemplateDeck {
public:
    TemplateDeck() {
        baseTemplates = {
            {CreatureType::WILK,      {"Wilk", 2, 3, 2, CostType::BLOOD, "wolf"}},
            {CreatureType::NIETOPERZ, {"Nietoperz", 1, 1, 1, CostType::BLOOD, "bat"}},
            {CreatureType::WRONA,     {"Wrona", 2, 2, 2, CostType::BLOOD, "raven"}},
            {CreatureType::WEZ,       {"Waz", 1, 3, 1, CostType::BLOOD, "snake"}},
            {CreatureType::OPOS,       {"Opos", 1, 1, 2, CostType::BONES, "opos"}},
            {CreatureType::KARALUCH,   {"Karaluch", 2, 1, 4, CostType::BONES, "roach"}},
            {CreatureType::WIEWIORKA, {"Wiewiorka", 1, 0, 0, CostType::BLOOD, "squirel"}}
        };
    }

    CardStats generateCardStats(CreatureType type) {
        if (baseTemplates.find(type) == baseTemplates.end()) {
            std::cout<<"Couldn't find type";
            return {"Error", 1, 0, 0, CostType::BLOOD, "placeholder"};
        }

        return baseTemplates[type];
    }

private:
    std::unordered_map<CreatureType, CardStats> baseTemplates;
};