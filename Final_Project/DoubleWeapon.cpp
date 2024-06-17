#include <allegro5/base.h>
#include <random>
#include <string>

#include "DirtyEffect.hpp"
#include "Monster.hpp"
#include "DoubleWeapon.hpp"
#include "Group.hpp"
#include "mode2.hpp"
#include "Point.hpp"

class Hero;

DoubleWeapon::DoubleWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-2.png", 1000, 1200, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {

}
void DoubleWeapon::OnExplode(Monster* monster) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), monster->Position.x, monster->Position.y));
}

DoubleWeapon2::DoubleWeapon2(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-10.png", 300, 1500, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {

}
void DoubleWeapon2::OnExplode(Monster* monster) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), monster->Position.x, monster->Position.y));
}