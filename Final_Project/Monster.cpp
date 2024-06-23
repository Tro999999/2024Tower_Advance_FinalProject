#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "AudioHelper.hpp"
#include "Bullet.hpp"
#include "DirtyEffect2.hpp"
#include "Monster.hpp"
#include "DiceMonster.hpp"
#include "DevilMonster.hpp"
#include "wolfknight.hpp"
#include "ExplosionEffect2.hpp"
#include "ShootEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"
#include "mode2.hpp"
#include "Turret.hpp"
#include "GoldenBullet.hpp"
#include "DoubleBullet.hpp"
#include "BlueNormalMonster.hpp"
#include "BossMonster.hpp"

#include "Monster.hpp"
#include "PoopMonster.hpp"


MODE2* Monster::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void Monster::OnExplode() {
    getMode2Scene()->EffectGroup->AddNewObject(new ExplosionEffect2(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect2("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}
Monster::Monster(std::string img, float x, float y, float radius, float speed, float hp, int money) :
        Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money), maxSpeed(speed), slowRemainTime(0) {
    CollisionRadius = radius;
    reachEndTime = 0;

}
void Monster::Hit(float damage) {
    hp -= damage;
    if(damage == 650){
        Monster::Slow(0.5, 3.0);
    }
    if (hp <= 0) {
        OnExplode();
        for (auto& it: lockedTurrets)
            it->Target = nullptr;
        for (auto& it: lockedBullets)
            it->Target = nullptr;
        getMode2Scene()->EarnMoney(money);
        getMode2Scene()->MonsterGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");

    }
    else if(speed == 30){
        //getMode2Scene()->MonsterGroup->AddNewObject(new PoopMonster(SpawnCoordinate.x, SpawnCoordinate.y));
        //getMode2Scene()->MonsterGroup->AddNewObject(new PoopMonster(30, 30));
    }

}
void Monster::UpdatePath(const std::vector<std::vector<int>>& mapDistance) {
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
        Engine::LOG(Engine::ERROR) << "Monster path finding error";
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
    path[0] = MODE2::EndGridPoint;
}
void Monster::Update(float deltaTime) {
    if (money == 500) {
        gentime += deltaTime;
        if(gentime >= 1){
            gentime = 0;
            Monster* Monster;
            getMode2Scene()->MonsterGroup->AddNewObject(Monster = new DevilMonster(Position.x, Position.y));
            Monster->UpdatePath(getMode2Scene()->mapDistance);
            Monster->Update(0);
        }
    }
    SlowEffect(deltaTime);

    float remainSpeed = speed * deltaTime;
    while (remainSpeed != 0) {
        if (path.empty()) {
            Hit(hp);
            getMode2Scene()->Hit();
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
void Monster::Draw() const {
    Sprite::Draw();
    if (MODE2::DebugMode) {
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}
void Monster::Slow(float slowFactor, float slowTime) {
    speed = maxSpeed * slowFactor;
    slowRemainTime = slowTime;
}
void Monster::SlowEffect(float deltaTime) {
    if (slowRemainTime > 0) {
        slowRemainTime -= deltaTime;
        if (slowRemainTime <= 0) {
            speed = maxSpeed;
        }
    }
}
//
void Monster::IsBoss(){
    if(speed == 20){
        getMode2Scene()->MonsterGroup->AddNewObject(new PoopMonster(-32, 32));
    }
}
