#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "GoldenWeapon.hpp"
#include "Group.hpp"
#include "PlugGunHero.hpp"
#include "mode2.hpp"
#include "Hero.hpp"
#include "HeroShootEffect.hpp"
#include "ExplosionEffect2.hpp"
#include "Point.hpp"

const int PlugGunHero::Price = 40;
PlugGunHero::PlugGunHero(float x, float y) :
        Hero("play/turret-6.png", x, y, 200, 45, 2, Price, 1) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}
void PlugGunHero::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getMode2Scene()->WeaponGroup->AddNewObject(new GoldenWeapon(Position + normalized * 36, diff, rotation, this));
    getMode2Scene()->EffectGroup->AddNewObject(new HeroShootEffect(Position.x + normalized.x * 36, Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");
}
