#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "GoldenBullet.hpp"
#include "DoubleBullet.hpp"
#include "Group.hpp"
#include "PlugGunTurret.hpp"
#include "PlayScene.hpp"
#include "ShootEffect.hpp"
#include "ExplosionEffect.hpp"
#include "Point.hpp"

const int PlugGunTurret::Price = 40;
PlugGunTurret::PlugGunTurret(float x, float y) :

    Turret("play/tower-base.png", "play/turret-6.png", x, y, 200, Price, 1) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}
void PlugGunTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new GoldenBullet(Position + normalized * 36, diff, rotation, this));
    getPlayScene()->EffectGroup->AddNewObject(new ShootEffect(Position.x + normalized.x * 36, Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");
}
