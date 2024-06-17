#ifndef HEROBUTTON_HPP
#define HEROBUTTON_HPP
#include <string>

#include "ImageButton.hpp"
#include "Sprite.hpp"

class MODE2;

class HeroButton : public Engine::ImageButton {
protected:
    MODE2* getMode2Scene();
public:
    int money;
    Engine::Sprite Base;
    Engine::Sprite Turret;
    HeroButton(std::string img, std::string imgIn, Engine::Sprite Base, Engine::Sprite Turret, float x, float y, int money);
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif // TURRETBUTTON_HPP
