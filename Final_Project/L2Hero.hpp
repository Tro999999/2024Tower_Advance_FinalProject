#ifndef L2HERO_HPP
#define L2HERO_HPP
#include "Hero.hpp"

class L2Hero : public Hero {
public:
    static const int Price;
    L2Hero(float x, float y);
    void CreateBullet() override;
};
/*
*/
#endif // PLUGGUNTURRET_HPP
