#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ScoreboardScene.hpp"
#include "Label.hpp"
#include "ImageButton.hpp"
#include "Point.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "Slider.hpp"
#include <fstream>
#include <iostream>
#include "StageSelectScene.hpp"
#include <algorithm> // For std::find_if and std::sort
#include <sstream> // For std::istringstream



void ScoreboardScene::Initialize() {
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
    LoadScores();

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Label("Time Spent (Seconds)", "pirulen.ttf", 48, halfW - 125, halfH / 2 - 50, 200, 20, 20, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label(std::to_string(currentPage), "pirulen.ttf", 60, w*0.5 , h-50, 200, 20, 20, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("start_scene/out.png", "start_scene/in.png", halfW * 1.3 + 100, halfH*1.5 - 500, 400, 100);
    prevBtn = new Engine::ImageButton("start_scene/out.png", "start_scene/in.png", w * 0.3-200, h - 100, 400, 100);
    nextBtn = new Engine::ImageButton("start_scene/out.png", "start_scene/in.png", w * 0.7-200, h - 100, 400, 100);
    //沒加上面這行會跳出畫面
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PlayOnClick, this, 1));
    //AudioHelper::StopSample(bgmInstance);
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW * 1.5 + 140 , halfH * 1.6 - 490 , 0, 0, 0, 255, 0.5, 0.5));
    // Load scores from file


    prevBtn->SetOnClickCallback(std::bind(&ScoreboardScene::OnPrevPageClick, this));
    AddNewControlObject(prevBtn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48, w * 0.3 , h - 50, 0, 0, 0, 255, 0.5, 0.5));

    nextBtn->SetOnClickCallback(std::bind(&ScoreboardScene::OnNextPageClick, this));
    AddNewControlObject(nextBtn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 48, w * 0.7 , h - 50, 0, 0, 0, 255, 0.5, 0.5));


    // Display scores
    DisplayScores();
}


void ScoreboardScene::LoadScores() {
    std::ifstream file("../Final_Project/resources/score.txt");
    std::string line;
    std::vector<std::pair<std::string, int>> scoreLines; // Store lines and their last number

    if (file.is_open()) {
        std::string line;
        std::string dateTime, playerScore;
        while (std::getline(file, line)) {
            if (!dateTime.empty()) {
                playerScore = line;
                scores.emplace_back(dateTime, playerScore);
                dateTime.clear();
            } else {
                dateTime = line;
            }
        }
        file.close();

        // Sort scores based on the numeric value of the player's time
        std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
            // Extract the time values from the playerScore strings
            double timeA = std::stod(a.second.substr(a.second.find_last_of(' ') + 1));
            double timeB = std::stod(b.second.substr(b.second.find_last_of(' ') + 1));
            return timeA < timeB;
        });
    } else {
        std::cerr << "Error opening score file for reading!" << std::endl;
    }
}

void ScoreboardScene::DisplayScores() {


    int start = (currentPage - 1) * itemsPerPage;
    int end = std::min(static_cast<int>(scores.size()), currentPage * itemsPerPage);
    int y = 100;

    for (int i = start; i < end; ++i) {
        auto dateTimeLabel = new Engine::Label(scores[i].first, "pirulen.ttf", 30, 500, y+200, 255, 255, 255, 255, 0.5, 0.5);
        auto playerScoreLabel = new Engine::Label(scores[i].second, "pirulen.ttf", 30, 1050, y + 200, 255, 255, 255, 255, 0.5, 0.5);
        AddNewObject(dateTimeLabel);
        AddNewObject(playerScoreLabel);
        y += 30;  // Increment y position for next score
    }
}


void ScoreboardScene::OnNextPageClick() {
    int totalPages = (scores.size() + itemsPerPage - 1) / itemsPerPage;
    if (currentPage < totalPages) {
        currentPage++;
    }
    else{
        currentPage = 1;
    }
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}

void ScoreboardScene::OnPrevPageClick() {
    int totalPages = (scores.size() + itemsPerPage - 1) / itemsPerPage;
    if (currentPage == 1) {
        currentPage = totalPages;
    }
    else {
        currentPage--;
    }
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}


void ScoreboardScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("mode1");
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
    // Clear scores vector and scene objects
    scores.clear();
    Clear();
}
