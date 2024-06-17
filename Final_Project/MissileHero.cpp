#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "Group.hpp"
#include "MissileWeapon.hpp"
#include "MissileHero.hpp"
#include "mode2.hpp"
#include "Point.hpp"

const int MissileHero::Price = 300;
MissileHero::MissileHero(float x, float y) :
        Hero("play/turret-3.png", x, y, 1000, 45, 2,  Price, 4) {
}
void MissileHero::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getMode2Scene()->WeaponGroup->AddNewObject(new MissileWeapon(Position + normalized * 10 - normal * 6, diff, rotation, this));
    getMode2Scene()->WeaponGroup->AddNewObject(new MissileWeapon(Position + normalized * 10 + normal * 6, diff, rotation, this));
    AudioHelper::PlayAudio("missile.wav");
}
