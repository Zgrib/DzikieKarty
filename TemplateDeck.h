#pragma once
#include <unordered_map>
#include <string>
#include "Card.h"

enum class CreatureType { WILK, NIETOPERZ, WRONA, WEZ, WIEWIORKA };

class TemplateDeck {
public:
    TemplateDeck() {
        baseTemplates = {
            {CreatureType::WILK,      {"Wilk", 2, 3, 2, CostType::BONES, "wolf"}},
            {CreatureType::NIETOPERZ, {"Nietoperz", 1, 1, 1, CostType::BLOOD, "bat"}},
            {CreatureType::WRONA,     {"Wrona", 2, 2, 2, CostType::BONES, "raven"}},
            {CreatureType::WEZ,       {"Waz", 1, 3, 2, CostType::BLOOD, "snake"}},
            {CreatureType::WIEWIORKA, {"Wiewiorka", 1, 0, 0, CostType::BONES, "squirel"}}
        };
    }

    CardStats generateCardStats(CreatureType type) {
        if (baseTemplates.find(type) == baseTemplates.end()) {
            return {"Error", 1, 0, 0, CostType::BLOOD, "placeholder"};
        }

        return baseTemplates[type];
    }

private:
    std::unordered_map<CreatureType, CardStats> baseTemplates;
};