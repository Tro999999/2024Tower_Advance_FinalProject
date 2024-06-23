#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "L4Hero.hpp"
#include "Group.hpp"
#include "RocketWeapon.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "HeroShootEffect.hpp"

const int L4Hero::Price = 200;
L4Hero::L4Hero(float x, float y) : Hero("play/turret-2.png", x, y, 600,45, 2, Price, 1)
{
    Anchor.y += 8.0f / GetBitmapHeight();
}


void L4Hero::CreateBullet()
{
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point original = diff.Normalize();
    Engine::Point middle(-original.y*10, original.x*10);
    Engine::Point RocketA = Position + middle ,RocketB = Position - middle ;
    Engine::Point bulletPosition = Position + original * 36;
    getMode2Scene()->WeaponGroup->AddNewObject(new RocketWeapon(RocketA + original * 36, diff, rotation, this));
    getMode2Scene()->WeaponGroup->AddNewObject(new RocketWeapon(RocketB + original * 36, diff, rotation, this));
    getMode2Scene()->EffectGroup->AddNewObject(new HeroShootEffect(bulletPosition.x, bulletPosition.y));
    AudioHelper::PlayAudio("gun.wav");
}


