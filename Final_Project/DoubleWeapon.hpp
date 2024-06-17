#ifndef DoubleWEAPON_HPP
#define DoubleWEAPON_HPP
#include "Weapon.hpp"

class Monster;
class Hero;
namespace Engine {
    struct Point;
}  // namespace Engine

class DoubleWeapon : public Weapon {
public:
    explicit DoubleWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void OnExplode(Monster* monster) override;
};

class DoubleWeapon2 : public Weapon {
public:
    explicit DoubleWeapon2(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void OnExplode(Monster* monster) override;
};
#endif // GOLDENBULLET_HPP
