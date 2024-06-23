#include <allegro5/base.h>
#include <random>
#include <string>

#include "DirtyEffect2.hpp"
#include "Monster.hpp"
#include "GoldenWeapon.hpp"
#include "Group.hpp"
#include "mode2.hpp"
#include "Point.hpp"

class Hero;

GoldenWeapon::GoldenWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-7.png", 1000, 650, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {

}
void GoldenWeapon::OnExplode(Monster* monster) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect2("play/dirty-1.png", dist(rng), monster->Position.x, monster->Position.y));
}
