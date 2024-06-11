#ifndef GOLDENBULLET_HPP
#define GOLDENBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}  // namespace Engine

class GoldenBullet : public Bullet {
public:
    explicit GoldenBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif // GOLDENBULLET_HPP
