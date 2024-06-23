#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "SettingScene.hpp"
#include "ScoreboardScene.hpp"
#include "Slider.hpp"
#include "StageSelect2.hpp"

void StageSelect2::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    // Stage 1 button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 250, halfH / 2 - 50, 500, 100);
    btn->SetOnClickCallback(std::bind(&StageSelect2::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Game Start", "pirulen.ttf", 48, halfW, halfH / 2 , 0, 0, 0, 255, 0.5, 0.5));

    // Stage 2 button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 250, halfH / 2 + 100, 500, 100);
    btn->SetOnClickCallback(std::bind(&StageSelect2::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Edit Map", "pirulen.ttf", 48, halfW, halfH / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));


    /*
    // Setting button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 400, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Setting", "pirulen.ttf", 48, halfW, halfH / 2 + 450, 0, 0, 0, 255, 0.5, 0.5));
*/

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW +320, halfH * 3 / 2 +100, 450, 130);
    btn->SetOnClickCallback(std::bind(&StageSelect2::PlayOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("quit", "pirulen.ttf", 48, halfW+550, halfH * 3/2 + 150, 200, 20, 20, 255, 0.5, 0.5));


    AddNewObject(new Engine::Label("Defeat enemies to defend your territory!", "pirulen.ttf", 35, halfW, halfH / 2 - 150, 200, 20, 20, 255, 0.5, 0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);



}
void StageSelect2::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void StageSelect2::PlayOnClick(int stage) {
    MODE2* scene = dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetScene("mode2"));

    //Engine::GameEngine::GetInstance().ChangeScene("play");
    /*
    if (stage == 3) {
        //PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("setting"));
        //scene->MapId = stage;
        Engine::GameEngine::GetInstance().ChangeScene("setting");
    }*/
    if(stage == 1){
        Engine::GameEngine::GetInstance().ChangeScene("mode2");
    }
    else if(stage == 2){
        Engine::GameEngine::GetInstance().ChangeScene("map-editor");
    }
    else if(stage == 4){
        Engine::GameEngine::GetInstance().ChangeScene("game-select");
    }

}
void StageSelect2::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelect2::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
