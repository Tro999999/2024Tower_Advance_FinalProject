#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "SmallRocket.hpp"
#include "Shovel.hpp"
#include "Group.hpp"
#include "L3Turret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ExplosionEffect.hpp"
#include "ShootEffect.hpp"

const int Shovel::Price = 0;
Shovel::Shovel(float x, float y) :
        Turret("play/tower-base.png", "play/Shovel.png", x, y, 400, Price, 2) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void Shovel::CreateBullet() {
    /*
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation =f.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet
     */
}
