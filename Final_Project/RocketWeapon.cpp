#include <allegro5/base.h>
#include <random>
#include <string>

#include "DirtyEffect.hpp"
#include "Monster.hpp"
#include "RocketWeapon.hpp"
#include "Group.hpp"
#include "mode2.hpp"
#include "Point.hpp"

class Hero;

RocketWeapon::RocketWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-3.png", 500, 500, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}
void RocketWeapon::OnExplode(Monster* monster) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-3.png", dist(rng), monster->Position.x, monster->Position.y));
}


