#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "L0Hero.hpp"
#include "Group.hpp"
#include "RocketWeapon.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "HeroShootEffect.hpp"
#include "GoldenWeapon.hpp"


const int L0Hero::Price = 400;
L0Hero::L0Hero(float x, float y) :
        Hero("play/Dragon.png", x, y, 200,45 , 2, Price, 0.1) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void L0Hero::CreateBullet()
{
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point bulletPosition = Position + normalized * 36;
    getMode2Scene()->WeaponGroup->AddNewObject(new GoldenWeapon(bulletPosition, diff, rotation, this));
    getMode2Scene()->EffectGroup->AddNewObject(new HeroShootEffect(bulletPosition.x, bulletPosition.y));
    AudioHelper::PlayAudio("gun.wav");
}


