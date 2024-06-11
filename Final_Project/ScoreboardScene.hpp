#ifndef SCOREBOARD_SCENE_HPP
#define SCOREBOARD_SCENE_HPP

#include "IScene.hpp"
#include <vector>
#include <string>
#include <allegro5/allegro_audio.h>
#include <memory>
#include "ImageButton.hpp"
#include "GameEngine.hpp"
#include "Label.hpp"
#include "IScene.hpp"

class ScoreboardScene : public Engine::IScene {
public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void PlayOnClick(int stage);
    void Terminate() override;

private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

    Engine::ImageButton* prevBtn;
    Engine::ImageButton* nextBtn;
    Engine::ImageButton* btn;
    std::vector<Engine::Label*> scoreLabels; // Add this line

    int currentPage = 1;
    const int itemsPerPage = 10;
    void LoadScores();
    void DisplayScores();
    void OnNextPageClick();
    void OnPrevPageClick();
    void ClearScoreLabels(); // Add this line
    std::vector<std::pair<std::string, std::string>> scores;  // Store date-time and score pairs
};

#endif
