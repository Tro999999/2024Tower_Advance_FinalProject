#include <cmath>
#include <string>

#include "ExplosionEffect2.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "mode2.hpp"
#include "Resources.hpp"


MODE2* ExplosionEffect2::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ExplosionEffect2::ExplosionEffect2(float x, float y) : Sprite("play/explosion-1.png", x, y), timeTicks(0) {
    for (int i = 1; i <= 5; i++) {
        bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/explosion-" + std::to_string(i) + ".png"));
    }
}
void ExplosionEffect2::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= timeSpan) {
        getMode2Scene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
    int phase = floor(timeTicks / timeSpan * bmps.size());
    bmp = bmps[phase];
    Sprite::Update(deltaTime);
}
