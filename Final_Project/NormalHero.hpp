#ifndef NORMALHERO_HPP
#define NORMALHERO_HPP
#include "Hero.hpp"

class NormalHero : public Hero {
public:
    static const int Price;
    NormalHero(int x, int y);
    void CreateBullet() override;
};
#endif // REDNORMALENEMY_HPP
