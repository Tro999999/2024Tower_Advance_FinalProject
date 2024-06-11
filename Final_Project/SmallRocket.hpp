#ifndef SMALLROCKET_HPP
#define SMALLROCKET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}  // namespace Engine

class SmallRocket : public Bullet {
public:
    explicit SmallRocket(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif


