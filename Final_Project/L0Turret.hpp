#ifndef L0TURRET_HPP
#define L0TURRET_HPP
#include "Turret.hpp"

class L0Turret : public Turret {
public:
    static const int Price;
    L0Turret(float x, float y);
    void CreateBullet() override;
};
#endif // PLUGGUNTURRET_HPP
