#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "IScene.hpp"
#include "input_field.hpp"

class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::string playerName;
    ALLEGRO_EVENT_QUEUE* event_queue{};
    std::unique_ptr<Engine::TextInput> input_field; // Add this line
public:
    explicit WinScene() = default;

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void Draw() const ;
};

#endif // WINSCENE_HPP
