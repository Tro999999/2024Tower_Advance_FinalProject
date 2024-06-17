#ifndef L4HERO_HPP
#define L4HERO_HPP
#include "Hero.hpp"

class L4Hero : public Hero {
public:
    static const int Price;
    L4Hero(float x, float y);
    void CreateBullet() override;
};
#endif // PLUGGUNTURRET_HPP
