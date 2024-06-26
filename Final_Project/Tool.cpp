#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>

#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Tool.hpp"
#include "PlugGunTurret.hpp"
#include "L0Turret.hpp"
#include "L2Turret.hpp"
#include "L5Turret.hpp"

#include "L3Turret.hpp"


PlayScene* Tool::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Tool::Tool(std::string imgTool, float x, float y, int price, float coolDown) :
        price(price), coolDown(coolDown), imgTool(imgTool, x, y) ,Sprite(imgTool, x, y){
}


int Tool::GetPrice(int id) const {
    if (id == 0) {
        return PlugGunTurret::Price/2;
    }
    else if (id == 1) {
        return L2Turret::Price/2;
    }
    else if (id == 2) {
        return L0Turret::Price/2;
    }
    else {
        return 0;
    }
}



void Tool::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    imgTool.Position = Position;
    imgTool.Tint = Tint;
    if (!Enabled)
        return;
}
void Tool::Draw() const {
    if (Preview) {
        al_draw_filled_circle(Position.x, Position.y, 0, al_map_rgba(0, 255, 0, 50));
    }
    //imgTool.Draw();
    Sprite::Draw();
}