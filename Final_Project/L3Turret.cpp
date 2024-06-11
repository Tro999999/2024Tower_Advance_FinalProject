#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "L3Turret.hpp"
#include "Group.hpp"
#include "SmallRocket.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ShootEffect.hpp"
#include "GoldenBullet.hpp"


const int L3Turret::Price = 100;
L3Turret::L3Turret(float x, float y) :
        Turret("play/tower-base.png", "play/turret-1.png", x, y, 600, Price, 3) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void L3Turret::CreateBullet()
{
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point bulletPosition = Position + normalized * 36;
    getPlayScene()->BulletGroup->AddNewObject(new SmallRocket(bulletPosition, diff, rotation, this));
    getPlayScene()->EffectGroup->AddNewObject(new ShootEffect(bulletPosition.x, bulletPosition.y));
    AudioHelper::PlayAudio("gun.wav");
}


