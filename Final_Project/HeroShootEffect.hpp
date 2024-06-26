#ifndef HEROSHOOTEFFECT_HPP
#define HEROSHOOTEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class MODE2;

class HeroShootEffect : public Engine::Sprite {
protected:
    MODE2* getMode2Scene();
    float timeTicks;
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
    float timeSpan = 0.5;
public:
    HeroShootEffect(float x, float y);
    void Update(float deltaTime) override;
};
#endif // ShootEFFECT_HPP
