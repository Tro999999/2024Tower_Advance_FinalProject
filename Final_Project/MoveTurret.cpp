#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "Group.hpp"
#include "MoveTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ExplosionEffect.hpp"

const int MoveTurret::Price = 0;
MoveTurret::MoveTurret(float x, float y) :

        Turret("play/tower-base.png", "play/MoveTool.png", x, y, 0, Price, 2) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void MoveTurret::CreateBullet() {

}
