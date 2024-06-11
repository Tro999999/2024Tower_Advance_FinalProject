#include <functional>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "StartScene.hpp"
#include "StageSelectScene.hpp"
#include "Resources.hpp"
#include "Point.hpp"

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    //AddNewObject(new Engine::Label("Bomber Man", "pirulen.ttf", 60, halfW, halfH / 2, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton* btn;
    Engine::ImageButton* sbtn;

    btn = new Engine::ImageButton("start_scene/out.png", "start_scene/in.png", halfW - 200, halfH , 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH +50 , 0, 0, 0, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("Tower Defense", "pirulen.ttf", 108, halfW, halfH/2 , 255, 255, 0, 255, 0.5, 0.5));

    // Setting button
    sbtn = new Engine::ImageButton("start_scene/out.png", "start_scene/in.png", halfW - 200, halfH / 2 + 400, 400, 100);
    sbtn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 2));
    AddNewControlObject(sbtn);
    AddNewObject(new Engine::Label("Setting", "pirulen.ttf", 48, halfW, halfH / 2 + 450, 0, 0, 0, 255, 0.5, 0.5));

/*
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    AudioHelper::PlayAudio("lose.wav");
*/
 }
void StartScene::Terminate() {
	IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {

   if(stage == 1)Engine::GameEngine::GetInstance().ChangeScene("stage-select");
   else Engine::GameEngine::GetInstance().ChangeScene("setting");
}