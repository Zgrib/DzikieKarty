#pragma once
#include <unordered_map>
#include <string>
#include "Card.h"

enum class CreatureType { WILK, NIETOPERZ, WRONA, WEZ, WIEWIORKA, KARALUCH };

class TemplateDeck {
public:
    TemplateDeck() {
        baseTemplates = {
            {CreatureType::WILK,      {"Wilk", 2, 3, 2, CostType::BLOOD, "wolf"}},
            {CreatureType::NIETOPERZ, {"Nietoperz", 1, 1, 1, CostType::BLOOD, "bat"}},
            {CreatureType::WRONA,     {"Wrona", 2, 2, 2, CostType::BLOOD, "raven"}},
            {CreatureType::WEZ,       {"Waz", 1, 3, 1, CostType::BLOOD, "snake"}},
            {CreatureType::WIEWIORKA, {"Wiewiorka", 1, 0, 0, CostType::BLOOD, "squirel"}},
            {CreatureType::KARALUCH, {"Karaluch", 1, 1, 2, CostType::BONES, "placeholder"}}
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