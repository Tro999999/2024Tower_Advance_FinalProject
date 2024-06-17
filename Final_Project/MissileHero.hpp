#ifndef MISSILEHERO_HPP
#define MISSILEHERO_HPP
#include "Hero.hpp"

class MissileHero: public Hero {
public:
    static const int Price;
    MissileHero(float x, float y);
    void CreateBullet() override;
};
#endif // MISSILETURRET_HPP
