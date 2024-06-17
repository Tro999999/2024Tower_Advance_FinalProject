#ifndef PLUGGUNHERO_HPP
#define PLUGGUNHERO_HPP
#include "Hero.hpp"

class PlugGunHero : public Hero {
public:
    static const int Price;
    PlugGunHero(float x, float y);
    void CreateBullet() override;
};
#endif
