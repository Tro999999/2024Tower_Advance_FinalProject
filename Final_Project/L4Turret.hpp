#ifndef L4TURRET_HPP
#define L4TURRET_HPP
#include "Turret.hpp"

class L4Turret : public Turret {
public:
    static const int Price;
    L4Turret(float x, float y);
    void CreateBullet() override;
};
#endif // PLUGGUNTURRET_HPP
