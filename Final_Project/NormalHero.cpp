#include <string>

#include "NormalHero.hpp"
#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "GoldenWeapon.hpp"
#include "Group.hpp"
#include "HeroShootEffect.hpp"
#include "ExplosionEffect2.hpp"
#include "Point.hpp"
#include "mode2.hpp"

const int NormalHero::Price = 400;

NormalHero::NormalHero(int x, int y) : Hero("play/enemy-4.png", x, y, 25, 45, 150, 30, 4) {
    Anchor.y += 8.0f / GetBitmapHeight();
}

void NormalHero::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getMode2Scene()->WeaponGroup->AddNewObject(new GoldenWeapon(Position + normalized * 36, diff, rotation, this));
    //getPlayScene()->EffectGroup->AddNewObject(new HeroShootEffect(Position.x + normalized.x * 36, Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");
}