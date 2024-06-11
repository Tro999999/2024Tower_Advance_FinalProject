#ifndef L3TURRET_HPP
#define L3TURRET_HPP
#include "Turret.hpp"

class L3Turret : public Turret {
public:
    static const int Price;
    L3Turret(float x, float y);
    void CreateBullet() override;
};

#endif // PLUGGUNTURRET_HPP
