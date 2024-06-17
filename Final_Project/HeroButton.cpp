#include <allegro5/color.h>

#include "GameEngine.hpp"
#include "IScene.hpp"
#include "mode2.hpp"
#include "Hero.hpp"
#include "Monster.hpp"
#include "HeroButton.hpp"

MODE2* HeroButton::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
HeroButton::HeroButton(std::string img, std::string imgIn, Engine::Sprite Base, Engine::Sprite Turret, float x, float y, int money) :
        ImageButton(img, imgIn, x, y), money(money), Base(Base), Turret(Turret) {
}
void HeroButton::Update(float deltaTime) {
    ImageButton::Update(deltaTime);
    if (getMode2Scene()->GetMoney() >= money) {
        Enabled = true;
        Base.Tint = Turret.Tint = al_map_rgba(255, 255, 255, 255);
    } else {
        Enabled = false;
        Base.Tint = Turret.Tint = al_map_rgba(0, 0, 0, 160);
    }
}
void HeroButton::Draw() const {
    ImageButton::Draw();
    Base.Draw();
    Turret.Draw();
}
