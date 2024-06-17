#include <allegro5/base.h>
#include <random>
#include <string>

#include "DirtyEffect.hpp"
#include "Monster.hpp"
#include "FireWeapon.hpp"
#include "Group.hpp"
#include "mode2.hpp"
#include "Point.hpp"

class Hero;

FireWeapon::FireWeapon(Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Weapon("play/bullet-1.png", 500, 100, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {

}
void FireWeapon::OnExplode(Monster* monster) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), monster->Position.x, monster->Position.y));
}

