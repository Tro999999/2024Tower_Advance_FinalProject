#ifndef MISSILEWEAPON_HPP
#define MISSILEWEAPON_HPP
#include <allegro5/base.h>
#include <list>

#include "Weapon.hpp"

class Monster;
class Hero;
namespace Engine {
    struct Point;
}  // namespace Engine

class MissileWeapon : public Weapon {
protected:
    const float rotateRadian = 2 * ALLEGRO_PI;
    std::list<Weapon*>::iterator lockedBulletIterator;
public:
    explicit MissileWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void Update(float deltaTime) override;
    void OnExplode(Monster* monster) override;
};
#endif
