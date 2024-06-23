#ifndef DIRTYEFFECT2_HPP
#define DIRTYEFFECT2_HPP
#include <string>

#include "Sprite.hpp"

class MODE2;

class DirtyEffect2 : public Engine::Sprite {
protected:
    float timeSpan;
    float alpha;
    MODE2* getMode2Scene();
public:
    DirtyEffect2(std::string img, float timeSpan, float x, float y);
    void Update(float deltaTime) override;
};
#endif // DIRTYEFFECT_HPP
