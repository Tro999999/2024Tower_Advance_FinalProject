#include <cmath>
#include <string>

#include "HeroShootEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "mode2.hpp"
#include "Resources.hpp"

MODE2* HeroShootEffect::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
HeroShootEffect::HeroShootEffect(float x, float y) : Sprite("play/shoot-1.png", x, y), timeTicks(0) {
    for (int i = 1; i <= 4; i++) {
        bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/shoot-" + std::to_string(i) + ".png"));
    }
}
void HeroShootEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= timeSpan) {
        getMode2Scene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
    int phase = floor(timeTicks / timeSpan * bmps.size());
    bmp = bmps[phase];
    Sprite::Update(deltaTime);
}
