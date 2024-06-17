#ifndef DICEMONSTER_2_HPP
#define DICEMONSTER_2_HPP

#include "DiceMonster_2.hpp"
#include "Monster.hpp"
class DiceMonster_2 : public Monster {
public:
    DiceMonster_2(int x, int y);
    void OnExplode() override ;
};



#endif


