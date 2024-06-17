#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "DoubleWeapon.hpp"
#include "GoldenWeapon.hpp"
#include "Group.hpp"
#include "L2Hero.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "ShootEffect.hpp"

const int L2Hero::Price = 60;
L2Hero::L2Hero(float x, float y) :

        Hero("play/turret-7.png", x, y, 550, 45, 2, Price, 1.5) {

    Anchor.y += 8.0f / GetBitmapHeight();
}
void L2Hero::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon(Position + normalized * 36, diff, rotation, this));

    getMode2Scene()->EffectGroup->AddNewObject(new ShootEffect(Position.x + normalized.x * 36 , Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");
}

