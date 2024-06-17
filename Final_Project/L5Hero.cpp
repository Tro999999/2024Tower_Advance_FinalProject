#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "L5Hero.hpp"
#include "Group.hpp"
#include "RocketWeapon.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "ShootEffect.hpp"
#include "DoubleWeapon.hpp"
#include "GoldenWeapon.hpp"


const int L5Hero::Price = 8000;
L5Hero::L5Hero(float x, float y) :
        Hero("play/mario.png", x, y, 250,45 , 2, Price, 0.005) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void L5Hero::CreateBullet()
{
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point perpendicular = Engine::Point(-normalized.y, normalized.x);

    // Change bullet position to the front of the gun barrel.
    getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon(Position + normalized * 36, diff, rotation, this));
    getMode2Scene()->EffectGroup->AddNewObject(new ShootEffect(Position.x + normalized.x * 36 , Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");


    if (CountBullet == 0 && i <= 1) {
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized * 36, diff - diff, 0.0, this));
        //getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized * 36 * 4, diff - diff, 0.0, this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular * 36, diff- diff, ALLEGRO_PI / 2 , this));
        //getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular * 36 * 4, diff- diff, ALLEGRO_PI / 2 , this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized* -1 * 36, diff- diff, ALLEGRO_PI  , this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular* -1 * 36, diff- diff, ALLEGRO_PI * 1.5, this));

        CountBullet = 2;
        i++;
    }
    else if (CountBullet == 0 && i <= 2) {

        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized * 36, diff - diff, 0.0, this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular * 36, diff- diff, ALLEGRO_PI / 2 , this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized* -1 * 36, diff- diff, ALLEGRO_PI  , this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular* -1 * 36, diff- diff, ALLEGRO_PI * 1.5, this));

        CountBullet = 2;
        i = 3;
    }
    else if (CountBullet == 0) {
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized * 36, diff - diff, 0.0, this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular * 36, diff- diff, ALLEGRO_PI / 2 , this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + normalized* -1 * 36, diff- diff, ALLEGRO_PI  , this));
        getMode2Scene()->WeaponGroup->AddNewObject(new DoubleWeapon2(Position + perpendicular* -1 * 36, diff- diff, ALLEGRO_PI * 1.5, this));

        CountBullet = 2;
    }
}


