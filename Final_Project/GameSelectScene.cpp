#include "GameSelectScene.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "Point.hpp"
#include "AudioHelper.hpp"

void GameSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // 添加标题 "Game Mode"
    AddNewObject(new Engine::Label("Game Mode", "pirulen.ttf", 60, halfW, halfH / 4, 255, 255, 255, 255, 0.5, 0.5));

    // 定义三个游戏模式的位置
    int imageY = halfH / 2;
    int buttonY = imageY + 200;
    int buttonWidth = 200;
    int buttonHeight = 60;

    std::vector<std::string> gameModes = {"Mode1", "Mode2", "Mode3"};
    std::vector<std::string> images = {"icon.png", "icon.png", "icon.png"}; // 替换为实际的图片路径
    int numModes = gameModes.size();

    for (int i = 0; i < numModes; ++i) {
        int x = halfW + (i - 1) * (w / 3);
        // 添加游戏模式图片
        AddNewObject(new Engine::ImageButton(images[i], images[i], x - 100, imageY, 200, 200));
        // 添加选择按钮
        Engine::ImageButton* selectButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", x - buttonWidth / 2, buttonY, buttonWidth, buttonHeight);
        selectButton->SetOnClickCallback(std::bind(&GameSelectScene::GameModeOnClick, this, i + 1));
        AddNewControlObject(selectButton);
        AddNewObject(new Engine::Label("Select", "pirulen.ttf", 24, x, buttonY + buttonHeight / 2, 0, 0, 0, 255, 0.5, 0.5));
    }
}

void GameSelectScene::Terminate() {
    IScene::Terminate();
}

void GameSelectScene::GameModeOnClick(int mode) {
    // 这里你可以根据 mode 的值切换到不同的游戏模式场景
    // 例如：Engine::GameEngine::GetInstance().ChangeScene("mode" + std::to_string(mode));
    if(mode == 1){
        Engine::GameEngine::GetInstance().ChangeScene("mode1");
    }
    else if(mode == 2){
        Engine::GameEngine::GetInstance().ChangeScene("mode2");
    }
    else{
        Engine::GameEngine::GetInstance().ChangeScene("mode3");
    }
}
