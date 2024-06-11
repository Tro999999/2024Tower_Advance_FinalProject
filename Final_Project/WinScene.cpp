#include <functional>
#include <string>
#include<fstream>
#include <ctime>
#include <iostream>
#include <sstream>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "WinScene.hpp"
#include "input_field.hpp"

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 + 10, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");

    playerName = "";
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    input_field = std::make_unique<Engine::TextInput>("pirulen.ttf", 24, halfW + 10, halfH /4 + 65, 300, 50, 255, 255, 255, 255, 0, 0, 0, 255, 0.5, 0.5);
    //AddNewObject(new Engine::TextInput("pirulen.ttf", 24, halfW , halfH /4 + 10, 300, 50, 255, 255, 255, 255, 0, 0, 0, 255, 0.5, 0.5));
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
    al_destroy_event_queue(event_queue);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }

    ALLEGRO_EVENT event;
    while (!al_is_event_queue_empty(event_queue)) {
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if(event.keyboard.keycode==ALLEGRO_KEY_ENTER) {
                playerName = input_field->GetText();
                auto playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
                float elapsedTime = playScene ? playScene->GetElapsedTime() : 0.0f;
                std::ofstream file;
                std::time_t currentTime = std::time(nullptr);
                std::tm* localTime = std::localtime(&currentTime);
                file.open("../Final_Project/resources/score.txt", std::ios_base::app);
                if (!file.is_open()) {
                    std::cerr << "Error opening file for writing!" << std::endl;
                    return;
                }
                file << "\n" << std::asctime(localTime) ;
                file << " "  << playerName << " " << elapsedTime;
                file.close();


                Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
            }
            input_field->OnKeyPress(event.keyboard.keycode);
        }
    }
}
void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::Draw() const {
    IScene::Draw();
    input_field->Draw();
    //al_draw_text(Engine::Resources::GetInstance().GetFont("pirulen.ttf", 36).get(), al_map_rgb(255, 255, 255), 20, 20, 0, std::to_string(ticks).c_str());
}
