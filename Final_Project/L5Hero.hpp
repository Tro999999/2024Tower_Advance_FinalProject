#ifndef L5HERO_HPP
#define L5HERO_HPP
#include "Hero.hpp"

class L5Hero : public Hero {
public:
    int i =0; // state
    static const int Price;
    L5Hero(float x, float y);
    void CreateBullet() override;
};

#endif
