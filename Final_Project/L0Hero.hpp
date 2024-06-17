#ifndef L0HERO_HPP
#define L0HERO_HPP
#include "Hero.hpp"

class L0Hero : public Hero {
public:
    static const int Price;
    L0Hero(float x, float y);
    void CreateBullet() override;
};

#endif
