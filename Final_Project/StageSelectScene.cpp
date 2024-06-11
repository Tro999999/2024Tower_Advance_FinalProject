#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "SettingScene.hpp"
#include "ScoreboardScene.hpp"
#include "Slider.hpp"
#include "StageSelectScene.hpp"

void StageSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    // Stage 1 button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 1", "pirulen.ttf", 48, halfW, halfH / 2 , 0, 0, 0, 255, 0.5, 0.5));

    // Stage 2 button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 2", "pirulen.ttf", 48, halfW, halfH / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));

    // Scoreboard button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 300, halfH / 2 + 250, 600, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 5));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 48, halfW, halfH / 2 + 300, 0, 0, 0, 255, 0.5, 0.5));

    /*
    // Setting button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 400, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Setting", "pirulen.ttf", 48, halfW, halfH / 2 + 450, 0, 0, 0, 255, 0.5, 0.5));
*/

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW +320, halfH * 3 / 2 +100, 450, 130);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("quit", "pirulen.ttf", 48, halfW+550, halfH * 3/2 + 150, 200, 20, 20, 255, 0.5, 0.5));


    AddNewObject(new Engine::Label("Defeat enemies to defend your territory!", "pirulen.ttf", 35, halfW, halfH / 2 - 150, 200, 20, 20, 255, 0.5, 0.5));
    /*
    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&StageSelectScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(new Ebel("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5, 0.5));
    */
     // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
    //上面這行要留
    //sliderBGM->SetValue(AudioHelper::BGMVolume);
    //sliderSFX->SetValue(AudioHelper::SFXVolume);


}
void StageSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void StageSelectScene::PlayOnClick(int stage) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = stage;
    //Engine::GameEngine::GetInstance().ChangeScene("play");
    /*
    if (stage == 3) {
        //PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("setting"));
        //scene->MapId = stage;
        Engine::GameEngine::GetInstance().ChangeScene("setting");
    }*/
    if(stage == 1){
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else if(stage == 2){
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else if(stage == 4){
        StageSelectScene::Terminate();
    }
    else if(stage == 5){
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
    }
}
void StageSelectScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelectScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
