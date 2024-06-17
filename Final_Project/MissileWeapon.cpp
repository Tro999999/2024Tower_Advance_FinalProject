#include <cmath>
#include <random>
#include <string>
#include <utility>

#include "DirtyEffect.hpp"
#include "Monster.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "MissileWeapon.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "Hero.hpp"

class Hero;

MissileWeapon::MissileWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-3.png", 100, 400, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent) {
}

void MissileWeapon::Update(float deltaTime) {
    if (!Target) {
        float minDistance = INFINITY;
        Monster* monster = nullptr;
        for (auto& it : getMode2Scene()->MonsterGroup->GetObjects()) {
            Monster* e = dynamic_cast<Monster*>(it);
            if (!e) continue; // Ensure dynamic cast was successful
            float distance = (e->Position - Position).Magnitude();
            if (distance < minDistance) {
                minDistance = distance;
                monster = e;
            }
        }
        if (!monster) {
            Weapon::Update(deltaTime);
            return;
        }
        Target = monster;
        Target->lockedBullets.push_back(this);
        lockedBulletIterator = std::prev(Target->lockedBullets.end());
    }
    Engine::Point originVelocity = Velocity.Normalize();
    Engine::Point targetVelocity = (Target->Position - Position).Normalize();
    float maxRotateRadian = rotateRadian * deltaTime;
    float cosTheta = originVelocity.Dot(targetVelocity);
    // Might have floating-point precision error.

    float radian = acos(cosTheta);
    if (std::abs(radian) <= maxRotateRadian)
        Velocity = targetVelocity;
    else
        Velocity = ((std::abs(radian) - maxRotateRadian) * originVelocity + maxRotateRadian * targetVelocity) / radian;
    Velocity = speed * Velocity.Normalize();
    Rotation = atan2(Velocity.y, Velocity.x) + ALLEGRO_PI / 2;
    Weapon::Update(deltaTime);
}

void MissileWeapon::OnExplode(Monster* monster) {

    if (Target) { // Check if Target is not null
        Target->lockedBullets.erase(lockedBulletIterator);
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(4, 10);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-3.png", dist(rng), monster->Position.x, monster->Position.y));
}
