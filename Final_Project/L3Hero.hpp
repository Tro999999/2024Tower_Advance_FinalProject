#ifndef L3HERO_HPP
#define L3HERO_HPP
#include "Hero.hpp"

class L3Hero : public Hero {
public:
    static const int Price;
    L3Hero(float x, float y);
    void CreateBullet() override;
};

#endif
