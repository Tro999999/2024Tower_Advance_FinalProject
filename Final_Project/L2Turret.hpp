#ifndef L2TURRET_HPP
#define L2TURRET_HPP
#include "Turret.hpp"

class L2Turret : public Turret {
public:
    static const int Price;
    L2Turret(float x, float y);
    void CreateBullet() override;
};
/*
*/
#endif // PLUGGUNTURRET_HPP
