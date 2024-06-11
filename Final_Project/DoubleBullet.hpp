#ifndef DoubleBULLET_HPP
#define DoubleBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}  // namespace Engine

class DoubleBullet : public Bullet {
public:
    explicit DoubleBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};

class DoubleBullet2 : public Bullet {
public:
    explicit DoubleBullet2(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif // GOLDENBULLET_HPP
