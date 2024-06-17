#include <allegro5/base.h>
#include <random>
#include <string>

#include "DirtyEffect.hpp"
#include "Monster.hpp"
#include "Group.hpp"
#include "LaserWeapon.hpp"
#include "mode2.hpp"
#include "Point.hpp"

class Hero;

LaserWeapon::LaserWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-2.png", 800, 50, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}
void LaserWeapon::OnExplode(Monster* monster) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), monster->Position.x, monster->Position.y));
}
