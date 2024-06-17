#ifndef WEAPON_HPP
#define WEAPON_HPP
#include <string>

#include "Sprite.hpp"

class Monster;
class MODE2;
class Hero;

namespace Engine {
    struct Point;
}  // namespace Engine

class Weapon : public Engine::Sprite {
protected:
    float speed , damage;

    MODE2* getMode2Scene();
    virtual void OnExplode(Monster* monster);
public:
    Hero* parent;
    Monster* Target = nullptr;
    explicit Weapon(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent);
    void Update(float deltaTime) override;

};
#endif
