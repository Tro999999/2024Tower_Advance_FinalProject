#ifndef MONSTER_HPP
#define MONSTER_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "Sprite.hpp"
#include "mode2.hpp"
#include "Monster.hpp"
#include "Weapon.hpp"
#include "Hero.hpp"

class Weapon;
class MODE2;
class Hero;


class Monster : public Engine::Sprite {
protected:
    std::vector<Engine::Point> path;
    float speed;
    float maxSpeed;
    float hp;
    float slowRemainTime;
    int money;
    MODE2* getMode2Scene();
public:
    float reachEndTime;
    float gentime = 0.0;
    float ticks;
    std::list<Hero*> lockedTurrets;
    std::list<Weapon*> lockedBullets;
    std::vector<std::vector<int>> mapDistance;
    Monster(std::string img, float x, float y, float radius, float speed, float hp, int money);
    void Hit(float damage);
    void UpdatePath(const std::vector<std::vector<int>>& mapDistance);
    void Update(float deltaTime) override;
    void Draw() const override;
    void Slow(float slowFactor, float slowTime);
    void SlowEffect(float deltaTime);
    void IsBoss();
    virtual void OnExplode();
};
#endif
