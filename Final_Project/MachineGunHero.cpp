#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "FireWeapon.hpp"
#include "Group.hpp"
#include "MachineGunHero.hpp"
#include "mode2.hpp"
#include "Point.hpp"

const int MachineGunHero::Price = 50;
MachineGunHero::MachineGunHero(float x, float y) :
        Hero("play/turret-7.png", x, y, 200, 45, 2,  Price, 0.5) {
    // Move center downward, since we the Hero head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void MachineGunHero::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getMode2Scene()->WeaponGroup->AddNewObject(new FireWeapon(Position + normalized * 36, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}
