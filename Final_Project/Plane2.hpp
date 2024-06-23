#ifndef PLANE2_HPP
#define PLANE2_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class MODE2;

class Plane2 : public Engine::Sprite {
protected:
    MODE2* getMode2Scene();
    const float timeSpanLight = 1;
    const float timeSpanShockwave = 1;
    const float shockWaveRadius = 180;
    const float minScale = 1.0f / 8;
    const float maxScale = 8;
    int stage;
    float timeTicks;
    float scale;
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
    std::shared_ptr<ALLEGRO_BITMAP> shockwave;
public:
    Plane2();
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif // PLANE_HPP