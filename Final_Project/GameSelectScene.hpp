#ifndef GAMESELECTSCENE_HPP
#define GAMESELECTSCENE_HPP

#include "IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>

class GameSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit GameSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void GameModeOnClick(int mode);
};

#endif // GAMESELECTSCENE_HPP
