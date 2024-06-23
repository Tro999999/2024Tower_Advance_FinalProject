#ifndef STAGESELECT2_HPP
#define STAGESELECT2_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "IScene.hpp"

class StageSelect2 final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit StageSelect2() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void OnClickScoreboardButton(); // Declare the button click handler
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif // STAGESELECTSCENE_HPP
