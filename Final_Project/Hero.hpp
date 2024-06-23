#ifndef Hero_HPP
#define Hero_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "mode2.hpp"
#include "Sprite.hpp"
#include "Monster.hpp"
#include "Hero.hpp"
#include "Weapon.hpp"

class Weapon;
class Monster;
class MODE2;


class Hero : public Engine::Sprite {
protected:
    std::vector<Engine::Point> heropath;
    std::list<Hero*>::iterator lockedTurretIterator;
    float speed;
    float coolDown;
    float reload = 0 , rotateRadian = 2 * ALLEGRO_PI;
    float maxSpeed;
    float hp;
    int money;
    MODE2* getMode2Scene();
    virtual void CreateBullet() = 0;
public:
    int price , CountBullet = 0;
    float reachEndTime;
    float gentime = 0.0;
    float ticks;
    Monster* Target = nullptr;
    std::vector<std::vector<int>> rmapDistance;
    Hero(std::string imgTurret, float x, float y, float radius, float speed, float hp, int price, float coolDown);
    void Hit(float damage);
    void UpdatePath(const std::vector<std::vector<int>>& mapDistance);
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnCollisionWithMonster();
    bool IsCollidingWith(Monster* monster);
    int GetPrice() const;
    virtual void OnExplode();
};
#endif // Hero_HPP
