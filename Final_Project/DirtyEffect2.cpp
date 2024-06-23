#include <allegro5/base.h>
#include <allegro5/color.h>
#include <random>

#include "DirtyEffect2.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "mode2.hpp"

MODE2* DirtyEffect2::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
DirtyEffect2::DirtyEffect2(std::string img, float timeSpan, float x, float y) : Sprite(img, x, y), timeSpan(timeSpan), alpha(1) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(-ALLEGRO_PI, ALLEGRO_PI);
    Rotation = dist(rng);
}
void DirtyEffect2::Update(float deltaTime) {
    alpha -= deltaTime / timeSpan;
    if (alpha <= 0) {
        getMode2Scene()->GroundEffectGroup->RemoveObject(objectIterator);
        return;
    }
    unsigned char r, g ,b, a = alpha * 255;
    al_unmap_rgb(Tint, &r, &g, &b);
    Tint = al_map_rgba(r, g, b, a);
    Sprite::Update(deltaTime);
}
