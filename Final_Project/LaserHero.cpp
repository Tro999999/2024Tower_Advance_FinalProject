#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "Group.hpp"
#include "LaserWeapon.hpp"
#include "LaserHero.hpp"
#include "mode2.hpp"
#include "Hero.hpp"
#include "Point.hpp"

const int LaserHero::Price = 20;
LaserHero::LaserHero(float x, float y) :
        Hero("play/turret-2.png", x, y, 300, 45, 2, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void LaserHero::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getMode2Scene()->WeaponGroup->AddNewObject(new LaserWeapon(Position + normalized * 36 - normal * 6, diff, rotation, this));
    getMode2Scene()->WeaponGroup->AddNewObject(new LaserWeapon(Position + normalized * 36 + normal * 6, diff, rotation, this));
    AudioHelper::PlayAudio("laser.wav");
}
