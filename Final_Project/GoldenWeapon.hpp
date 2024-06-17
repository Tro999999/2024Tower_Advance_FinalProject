#ifndef GOLDENWEAPON_HPP
#define GOLDENWEAPON_HPP
#include "Weapon.hpp"

class Monster;
class Hero;
namespace Engine {
    struct Point;
}  // namespace Engine

class GoldenWeapon : public Weapon {
public:
    explicit GoldenWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void OnExplode(Monster* monster) override;
};
#endif // GOLDENBULLET_HPP
