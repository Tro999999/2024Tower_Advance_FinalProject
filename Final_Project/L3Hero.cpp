#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "L3Hero.hpp"
#include "Group.hpp"
#include "RocketWeapon.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "HeroShootEffect.hpp"
#include "GoldenWeapon.hpp"


const int L3Hero::Price = 100;
L3Hero::L3Hero(float x, float y) :
        Hero("play/turret-1.png", x, y, 600,45 , 2, Price, 3) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void L3Hero::CreateBullet()
{
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point bulletPosition = Position + normalized * 36;
    getMode2Scene()->WeaponGroup->AddNewObject(new RocketWeapon(bulletPosition, diff, rotation, this));
    getMode2Scene()->EffectGroup->AddNewObject(new HeroShootEffect(bulletPosition.x, bulletPosition.y));
    AudioHelper::PlayAudio("gun.wav");
}


