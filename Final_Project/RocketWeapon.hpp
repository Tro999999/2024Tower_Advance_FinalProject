#ifndef ROCKETWEAPON_HPP
#define ROCKETWEAPON_HPP
#include "Weapon.hpp"

class Monster;
class Hero;
namespace Engine {
    struct Point;
}  // namespace Engine

class RocketWeapon : public Weapon {
public:
    explicit RocketWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void OnExplode(Monster* monster) override;
};
#endif


