#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "AudioHelper.hpp"
#include "Bullet.hpp"
#include "DirtyEffect.hpp"
#include "wolfknight.hpp"
#include "ExplosionEffect.hpp"
#include "ShootEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"
#include "GoldenBullet.hpp"
#include "DoubleBullet.hpp"
#include "Hero.hpp"
#include "Weapon.hpp"
#include "Monster.hpp"
#include "mode2.hpp"


MODE2* Hero::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Hero::OnExplode() {
    getMode2Scene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}
Hero::Hero(std::string img, float x, float y, float radius, float speed, float hp, int price, float coolDown) :
        Engine::Sprite(img, x, y), speed(speed), hp(hp), maxSpeed(speed), price(price), coolDown(coolDown) {
    CollisionRadius = radius;
    reachEndTime = 0;
}
void Hero::Hit(float damage) {
    hp -= damage;
    if (hp <= 0) {
        OnExplode();
        getMode2Scene()->HeroGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");

    }
}
void Hero::UpdatePath(const std::vector<std::vector<int>>& mapDistance) {
    int x = static_cast<int>(floor(Position.x / MODE2::BlockSize));
    int y = static_cast<int>(floor(Position.y / MODE2::BlockSize));
    if (x < 0) x = 0;
    if (x >= MODE2::MapWidth) x = MODE2::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= MODE2::MapHeight) y = MODE2::MapHeight - 1;
    Engine::Point pos(x, y);
    int num = mapDistance[y][x];
    if (num == -1) {
        num = 0;
        Engine::LOG(Engine::ERROR) << "Hero path finding error";
    }
    path = std::vector<Engine::Point>(num + 1);
    while (num != 0) {
        std::vector<Engine::Point> nextHops;
        for (auto& dir : MODE2::directions) {
            int x = pos.x + dir.x;
            int y = pos.y + dir.y;
            if (x < 0 || x >= MODE2::MapWidth || y < 0 || y >= MODE2::MapHeight || mapDistance[y][x] != num - 1)
                continue;
            nextHops.emplace_back(x, y);
        }
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)];
        path[num] = pos;
        num--;
    }
    path[0] = MODE2::SpawnGridPoint;
}
void Hero::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    MODE2* scene = getMode2Scene();
    float remainSpeed = speed * deltaTime;
    //Turret
    if (Target && price != 8000) {
        Engine::Point diff = Target->Position - Position;
        if (diff.Magnitude() > CollisionRadius) {
            Target->lockedTurrets.erase(lockedTurretIterator);
            Target = nullptr;
            lockedTurretIterator = std::list<Hero*>::iterator();
        }
    }
    if(price == 8000){
        CreateBullet();
    }
    if (Target) {

    }
    else{
        for (auto& it : scene->MonsterGroup->GetObjects()) {
            Engine::Point diff = it->Position - Position;
            if (diff.Magnitude() <= CollisionRadius) {
                Target = dynamic_cast<Monster*>(it);
                Target->lockedTurrets.push_back(this);
                lockedTurretIterator = std::prev(Target->lockedTurrets.end());
                break;
            }
        }
    }
    if (Target && price != 8000) {
        Engine::Point originRotation = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
        Engine::Point targetRotation = (Target->Position - Position).Normalize();
        float maxRotateRadian = rotateRadian * deltaTime , cosTheta = originRotation.Dot(targetRotation);

        if (cosTheta > 1) cosTheta = 1;
        else if (cosTheta < -1) cosTheta = -1;
        float radian = acos(cosTheta);
        Engine::Point rotation;
        if (abs(radian) <= maxRotateRadian)
            rotation = targetRotation;
        else
            rotation = ((abs(radian) - maxRotateRadian) * originRotation + maxRotateRadian * targetRotation) / radian;

        Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI / 2;
        reload -= deltaTime;
        if (reload <= 0) {
            reload = coolDown;
            CreateBullet();
        }
    }
    //Turret
    while (remainSpeed != 0) {
        if (path.empty()) {
            Hit(hp);
            getMode2Scene()->Attack();
            reachEndTime = 0;
            return;
        }
        Engine::Point target = path.back() * MODE2::BlockSize + Engine::Point(MODE2::BlockSize / 2, MODE2::BlockSize / 2);
        Engine::Point vec = target - Position;
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * MODE2::BlockSize - remainSpeed) / speed;
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed - vec.Magnitude() > 0) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        }
        else {
            Velocity = normalized * remainSpeed / deltaTime;
            remainSpeed = 0;
        }
    }
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}
void Hero::Draw() const {
    Sprite::Draw();
    if (MODE2::DebugMode) {
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}

int Hero::GetPrice() const {
    return price;
}
