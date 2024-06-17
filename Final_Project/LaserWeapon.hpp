#ifndef LASERBULLET_HPP
#define LASERBULLET_HPP
#include "Weapon.hpp"

class Monster;
class Hero;
namespace Engine {
    struct Point;
}  // namespace Engine

class LaserWeapon : public Weapon {
public:
    explicit LaserWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void OnExplode(Monster* Monster) override;
};
#endif // LASERBULLET_HPP
