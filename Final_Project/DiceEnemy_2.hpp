#ifndef DICEENEMY_2_HPP
#define DICEENEMY_2_HPP

#include "DiceEnemy_2.hpp"
#include "Enemy.hpp"
class DiceEnemy_2 : public Enemy {
public:
    DiceEnemy_2(int x, int y);
    void OnExplode() override ;
};



#endif


