#ifndef EXPLOSIONEFFECT2_HPP
#define EXPLOSIONEFFECT2_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class MODE2;

class ExplosionEffect2 : public Engine::Sprite {
protected:
    MODE2* getMode2Scene();
    float timeTicks;
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
    float timeSpan = 0.5;
public:
    ExplosionEffect2(float x, float y);
    void Update(float deltaTime) override;
};
#endif // EXPLOSIONEFFECT_HPP
