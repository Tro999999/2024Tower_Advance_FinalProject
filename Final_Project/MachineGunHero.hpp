#ifndef MACHINEGUNHERO_HPP
#define MACHINEGUNHERO_HPP
#include "Hero.hpp"

class MachineGunHero: public Hero {
public:
    static const int Price;
    MachineGunHero(float x, float y);
    void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
