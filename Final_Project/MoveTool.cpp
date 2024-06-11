#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "Group.hpp"
#include "MoveTool.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ExplosionEffect.hpp"

const int MoveTool::Price = 0;
MoveTool::MoveTool(float x, float y) :
//Tool("play/Move.png", x, y, Price, 2)
        Turret("play/tower-base.png", "play/MoveTool.png", x, y, 0, Price, 0) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void MoveTool::CreateBullet() {

}

