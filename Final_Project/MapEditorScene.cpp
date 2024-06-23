#include "MapEditorScene.hpp"
#include <allegro5/allegro_primitives.h>
#include "GameEngine.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include <fstream>
#include <stdexcept>

void MapEditorScene::Initialize() {
    // 初始化地图大小
    MapWidth = 20;
    MapHeight = 13;
    BlockSize = 64;

    // 读取地图文件
    std::string filename = "resources/map4.txt";
    mapData = std::vector<std::vector<int>>(MapHeight, std::vector<int>(MapWidth, 0));
    std::ifstream fin(filename);
    if (!fin.is_open()) throw std::runtime_error("Failed to open map file");

    for (int i = 0; i < MapHeight; ++i) {
        for (int j = 0; j < MapWidth; ++j) {
            char ch;
            fin >> ch;
            mapData[i][j] = (ch == '1');
        }
    }
    fin.close();

    // 添加退出按钮说明
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    // 计算右侧空白区域的宽度
    int rightBlankWidth = w - (MapWidth * BlockSize);

    // 定义提示位置在右手边空白区域的合适位置
    int labelX = w - (rightBlankWidth / 2) - 50;  // 向左调整50像素
    int labelY = h / 2;
    // 创建提示 Label
    AddNewObject(new Engine::Label("S to save", "pirulen.ttf", 24, labelX + 30, labelY, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("ESC to exit", "pirulen.ttf", 24, labelX + 30, labelY + 50, 255, 255, 255, 255, 0.5, 0.5));
}

void MapEditorScene::Update(float deltaTime) {
    IScene::Update(deltaTime);
}

void MapEditorScene::Draw() const {
    IScene::Draw();
    // 绘制地图
    for (int y = 0; y < MapHeight; ++y) {
        for (int x = 0; x < MapWidth; ++x) {
            ALLEGRO_COLOR color = mapData[y][x] ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 0, 0);
            al_draw_filled_rectangle(x * BlockSize, y * BlockSize, (x + 1) * BlockSize, (y + 1) * BlockSize, color);
            al_draw_rectangle(x * BlockSize, y * BlockSize, (x + 1) * BlockSize, (y + 1) * BlockSize, al_map_rgb(255, 255, 255), 2);
        }
    }
}


// OnMouseDown function
void MapEditorScene::OnMouseDown(int button, int mx, int my) {
    if (button == 1) { // 左键点击
        int x = mx / BlockSize;
        int y = my / BlockSize;
        if (x >= 0 && x < MapWidth && y >= 0 && y < MapHeight) {
            // 切换墙壁和地面
            mapData[y][x] = !mapData[y][x];
        }
    }
}

void MapEditorScene::OnKeyDown(int keyCode) {
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        // 返回 stage-select 场景
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
    if (keyCode == ALLEGRO_KEY_S) {
        // 保存地图
        SaveMap();
    }
}

void MapEditorScene::LoadMap() {
    std::ifstream fin(mapFile);
    mapData = std::vector<std::vector<int>>(MapHeight, std::vector<int>(MapWidth));
    if (fin.is_open()) {
        for (int y = 0; y < MapHeight; ++y) {
            for (int x = 0; x < MapWidth; ++x) {
                char c;
                fin >> c;
                mapData[y][x] = c - '0';
            }
        }
        fin.close();
    } else {
        // 默认初始化
        for (int y = 0; y < MapHeight; ++y) {
            for (int x = 0; x < MapWidth; ++x) {
                mapData[y][x] = 0;
            }
        }
    }
}

// SaveMap function
void MapEditorScene::SaveMap() {
    std::string filename = "resources/map4.txt";
    std::ofstream fout(filename);
    if (!fout.is_open()) throw std::runtime_error("Failed to open map file for writing");

    for (int y = 0; y < MapHeight; ++y) {
        for (int x = 0; x < MapWidth; ++x) {
            fout << (mapData[y][x] ? '1' : '0');
        }
        fout << "\n";
    }
    fout.close();
}

void MapEditorScene::ToggleBlock(int x, int y) {
    mapData[y][x] = !mapData[y][x];
}
