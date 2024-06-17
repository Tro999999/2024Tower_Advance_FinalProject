#ifndef LASERHERO_HPP
#define LASERHERO_HPP
#include "Hero.hpp"

class LaserHero: public Hero {
public:
    static const int Price;
    LaserHero(float x, float y);
    void CreateBullet() override;
};
#endif // LASERTURRET_HPP
