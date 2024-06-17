#ifndef FIREWEAPON_HPP
#define FIREWEAPON_HPP
#include "Weapon.hpp"

class Monster;
class Hero;
namespace Engine {
    struct Point;
}  // namespace Engine

class FireWeapon : public Weapon{
public:
    explicit FireWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void OnExplode(Monster* monster) override;
};
#endif // FIREBULLET_HPP
