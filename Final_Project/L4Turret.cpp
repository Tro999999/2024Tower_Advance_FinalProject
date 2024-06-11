#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "L4Turret.hpp"
#include "Group.hpp"
#include "SmallRocket.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ShootEffect.hpp"

const int L4Turret::Price = 200;
L4Turret::L4Turret(float x, float y) : Turret("play/tower-base.png", "play/turret-2.png", x, y, 600, Price, 1)
{
    Anchor.y += 8.0f / GetBitmapHeight();
}


void L4Turret::CreateBullet()
{
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point original = diff.Normalize();
    Engine::Point middle(-original.y*10, original.x*10);
    Engine::Point RocketA = Position + middle ,RocketB = Position - middle ;
    Engine::Point bulletPosition = Position + original * 36;
    getPlayScene()->BulletGroup->AddNewObject(new SmallRocket(RocketA + original * 36, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new SmallRocket(RocketB + original * 36, diff, rotation, this));
    getPlayScene()->EffectGroup->AddNewObject(new ShootEffect(bulletPosition.x, bulletPosition.y));
    AudioHelper::PlayAudio("gun.wav");
}


