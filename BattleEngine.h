#include <vector>
#include "Card.h"

#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H
class BattleEngine{
private:
    //może lepiej aby to były sloty na kartę a nie po prostu karty???
    //to chyba zależy, tak też można
    //std::vector<std::vector<Card*>> board;
    Card* board[4][2];


public:
    void PrepareBattle(){



    }

    void EndTurn(){




    }

    void EndBattle(){



    }

};

#endif // BATTLEENGINE_H
