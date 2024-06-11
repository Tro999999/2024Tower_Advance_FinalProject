#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "DoubleBullet.hpp"
#include "GoldenBullet.hpp"
#include "Group.hpp"
#include "L2Turret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ShootEffect.hpp"

const int L2Turret::Price = 60;
L2Turret::L2Turret(float x, float y) :

        Turret("play/tower-base.png", "play/turret-7.png", x, y, 550, Price, 1.5) {

    Anchor.y += 8.0f / GetBitmapHeight();
}
void L2Turret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet(Position + normalized * 36, diff, rotation, this));

    getPlayScene()->EffectGroup->AddNewObject(new ShootEffect(Position.x + normalized.x * 36 , Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");
}

