#ifndef L5TURRET_HPP
#define L5TURRET_HPP
#include "Turret.hpp"

class L5Turret : public Turret {
public:
    int i =0; // state
    static const int Price;
    L5Turret(float x, float y);
    void CreateBullet() override;
};
#endif // PLUGGUNTURRET_HPP
