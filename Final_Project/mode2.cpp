#include <allegro5/allegro.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <iostream>

#include "AudioHelper.hpp"
#include "DirtyEffect2.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "mode2.hpp"
// Hero
#include "PlugGunHero.hpp"
#include "L0Hero.hpp"
#include "L2Hero.hpp"
#include "L3Hero.hpp"
#include "L4Hero.hpp"
#include "L5Hero.hpp"
#include "LaserHero.hpp"
#include "MachineGunHero.hpp"
#include "MissileHero.hpp"
#include "Plane2.hpp"

#include "SettingScene.hpp"
#include "ScoreboardScene.hpp"
#include "NormalHero.hpp"
#include "Weapon.hpp"

//Tool
#include "MoveTool.hpp"
#include "Shovel.hpp"
// Monster
#include "PoopMonster.hpp"
#include "DevilMonster.hpp"
#include "wolfknight.hpp"
#include "RedNormalMonster.hpp"
#include "BossMonster.hpp"
#include "DiceMonster.hpp"
#include "DiceMonster_2.hpp"
#include "BlueNormalMonster.hpp"
#include "Resources.hpp"
#include "Sprite.hpp"
#include "Hero.hpp"
#include "Bullet.hpp"
#include "Monster.hpp"
#include "Hero.hpp"
#include "HeroButton.hpp"
#include "LOG.hpp"
#include "Collider.hpp"
#include "Point.hpp"
#include "Tool.hpp"
#include "ToolButton.hpp"

int Check = 0;


bool MODE2::DebugMode = false;
int UTool = 0;
const std::vector<Engine::Point> MODE2::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int MODE2::MapWidth = 20, MODE2::MapHeight = 13;
const int MODE2::BlockSize = 64;
const float MODE2::DangerTime = 7.61;
const Engine::Point MODE2::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point MODE2::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
float vol =  AudioHelper::SFXVolume;


//const std::vector<int> MODE2::code = { ALLEGRO_KEY_UP };
const std::vector<int> MODE2::code = { ALLEGRO_KEY_Z, ALLEGRO_KEY_X, ALLEGRO_KEY_C,ALLEGRO_KEY_V, ALLEGRO_KEY_B, ALLEGRO_KEY_N };
Engine::Point MODE2::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void MODE2::Initialize() {
    isTool = false;
    mapState.clear();
    rmapState.clear();
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 10;
    money = 150;
    SpeedMult = 1;
    elapsedTime = 0;
    lastUpdateTime = 0;
    currentSelectedButtonID = -1;
    // Add groups from bottom to top.
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    AddNewObject(ToolGroup = new Group());
    AddNewObject(HeroGroup = new Group());
    AddNewObject(WeaponGroup = new Group());
    AddNewObject(MonsterGroup = new Group());
    // Should support buttons.
    AddNewControlObject(UIGroup = new Group());
    ReadMap();

    startTime = al_get_time();

    ReadMonsterWave();
    mapDistance = CalculateBFSDistance();
    rmapDistance = CalculateDistance();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    // Preload Lose Scene
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");


    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;


    AddNewObject(new Engine::Label("<esc to exit>", "pirulen.ttf", 25, halfW * 1.7 + 75 , halfH * 1.6 + 40 , 0, 0, 0, 255, 0.5, 0.5));

    MODE2* scene = dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetScene("mode2"));
    if (!mute)
        bgmInstance = AudioHelper::PlaySample("play2.ogg", true, AudioHelper::BGMVolume);
    else
        bgmInstance = AudioHelper::PlaySample("play.ogg", true, 0.0);
}
void MODE2::Terminate() {
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void MODE2::Update(float deltaTime) {
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    IScene::Update(deltaTime);
    //elapsedTime = al_get_time() - startTime;
    elapsedTime += deltaTime * SpeedMult;

    if (elapsedTime - lastUpdateTime >= 1) {
        lastUpdateTime = elapsedTime;
        Time->Text = std::to_string(static_cast<int>(elapsedTime));
    }


    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;
    // Calculate danger zone.
    std::vector<float> reachEndTimes;
    for (auto& it : MonsterGroup->GetObjects()) {
        reachEndTimes.push_back(dynamic_cast<Monster*>(it)->reachEndTime);
    }

    std::sort(reachEndTimes.begin(), reachEndTimes.end());
    float newDeathCountDown = -1;
    int danger = lives;
    if(1) {
        for (auto &it: reachEndTimes) {
            if (it <= DangerTime) {
                danger--;
                if (danger <= 0) {
                    // Death Countdown
                    float pos = DangerTime - it;
                    if (it > deathCountDown) {
                        // Restart Death Count Down BGM.
                        AudioHelper::StopSample(deathBGMInstance);
                        if (SpeedMult != 0)
                            deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume,
                                                                       pos);
                    }
                    float alpha = pos / DangerTime;
                    alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
                    dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
                    newDeathCountDown = it;
                    break;
                }
            }
        }
    }

    deathCountDown = newDeathCountDown;
    if (SpeedMult == 0)
        AudioHelper::StopSample(deathBGMInstance);
    if (deathCountDown == -1 && lives > 0) {
        AudioHelper::StopSample(deathBGMInstance);
        dangerIndicator->Tint.a = 0;
    }
    if (SpeedMult == 0)
        deathCountDown = -1;
    if(1) {
        for (int i = 0; i < SpeedMult; i++) {
            IScene::Update(deltaTime);
            ticks += deltaTime;
            if (MonsterWaveData.empty()) {
                if (MonsterGroup->GetObjects().empty()) {
                    Engine::GameEngine::GetInstance().ChangeScene("win");
                }
                continue;
            }

            auto current = MonsterWaveData.front();
            if (ticks < current.second)
                continue;
            ticks -= current.second;
            MonsterWaveData.pop_front();
            const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2,
                                                                SpawnGridPoint.y * BlockSize + BlockSize / 2);

            Monster *Monster;
            switch (current.first) {
                case 1:
                    MonsterGroup->AddNewObject(Monster = new RedNormalMonster(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 2:
                    //std::cout << "Monster : 2\n" ;
                    MonsterGroup->AddNewObject(Monster = new BlueNormalMonster(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 3:
                    MonsterGroup->AddNewObject(Monster = new DiceMonster_2(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 4:
                    MonsterGroup->AddNewObject(Monster = new BossMonster(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                    //case 5 6
                case 7:
                    MonsterGroup->AddNewObject(Monster = new DevilMonster(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 5:
                    MonsterGroup->AddNewObject(Monster = new PoopMonster(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 6:
                    MonsterGroup->AddNewObject(Monster = new DiceMonster(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 8:
                    //MonsterGroup->AddNewObject(Monster = new wolfknight(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                default:
                    continue;
            }
            Monster->UpdatePath(mapDistance);
            // Compensate the time lost.
            Monster->Update(ticks);
        }
    }
}
void MODE2::Draw() const {
    IScene::Draw();
    if(1) {
        if (DebugMode) {
            // Draw reverse BFS distance on all reachable blocks.
            for (int i = 0; i < MapHeight; i++) {
                for (int j = 0; j < MapWidth; j++) {
                    if (mapDistance[i][j] != -1) {
                        // Not elegant nor efficient, but it's quite enough for debugging.
                        Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize,
                                            (i + 0.5) * BlockSize);
                        label.Anchor = Engine::Point(0.5, 0.5);
                        label.Draw();
                    }
                }
            }
        }
    }
}
void MODE2::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
}
void MODE2::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}
void MODE2::OnMouseUp(int button, int mx, int my) {
    const Engine::Point SpawnCoordinate = Engine::Point(EndGridPoint.x * BlockSize + BlockSize / 2,
                                                      EndGridPoint.y * BlockSize + BlockSize / 2);
    IScene::OnMouseUp(button, mx, my);
    Hero* hero;
    if (!imgTarget->Visible)
        return;
    const int x = mx / BlockSize, y = my / BlockSize ;
}
void MODE2::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_TAB) {
        MODE2::DebugMode ^= 1;

    }

    else {
        keyStrokes.push_back(keyCode);
        if (keyStrokes.size() > code.size())
            keyStrokes.pop_front();
        // Active a plane : EffectGroup->AddNewObject(new Plane());
        // Earn money : money += 10000;
        std::vector<int> compare;
        for (auto &i: keyStrokes) compare.push_back((i));
        if (compare == MODE2::code){
            EffectGroup->AddNewObject(new Plane2());
            money =  money + 10000;
        }
    }
    if (keyCode == ALLEGRO_KEY_Q) {
        // Hotkey for PlugGunHero.
        UIBtnClicked(0);
    }
    else if (keyCode == ALLEGRO_KEY_W) {
        // Hotkey for new Hero.
        UIBtnClicked(1);
    }
    else if (keyCode == ALLEGRO_KEY_A) {
        // Hotkey for new Hero.
        UIBtnClicked(10);
    }
    else if (keyCode == ALLEGRO_KEY_E) {
        // Hotkey for new Hero.
        UIBtnClicked(2);
    }
    else if (keyCode == ALLEGRO_KEY_T) {
        // Hotkey for new Hero.
        UIBtnClicked(4);
    }
    else if (keyCode == ALLEGRO_KEY_S) {
        // Hotkey for new Hero.
        UIBtnClicked(11);
    }
    else if (keyCode == ALLEGRO_KEY_R) {
        // Hotkey for new Hero.
        UIBtnClicked(3);
    }
    else if (keyCode == ALLEGRO_KEY_D) {
        // Hotkey for new Hero.
        UIBtnClicked(12);
    }
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        SpeedMult = keyCode - ALLEGRO_KEY_0;
    }
    else if (keyCode == ALLEGRO_KEY_M) {
        // Hotkey for mute / unmute.
        if (mute) {
            AudioHelper::SFXVolume = vol;
            AudioHelper::ChangeSampleVolume(bgmInstance, AudioHelper::BGMVolume);
        }
        else{
            AudioHelper::ChangeSampleVolume(bgmInstance, 0.0);
            AudioHelper::SFXVolume = 0;
        }
        mute ^= 1;
    }
    else if(keyCode == ALLEGRO_KEY_ESCAPE){
        AudioHelper::ChangeSampleVolume(bgmInstance, 0.0);
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
}
void MODE2::Hit() {
    UILives->Text = std::string("Life ") + std::to_string(--lives);
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose");
    }
}
int MODE2::GetMoney() const {
    return money;
}
void MODE2::EarnMoney(int money) {
    this->money += money;
    UIMoney->Text = std::string("$") + std::to_string(this->money);
}



void MODE2::ReadMap() {
    std::string filename = std::string("resources/map4.txt") ;
    // Read map file.
    char s;
    std::vector<bool> mapData;
    std::ifstream fin(filename);
    while (fin >> s) {
        switch (s) {
            case '0': mapData.push_back(false); break;
            case '1': mapData.push_back(true); break;
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    // Validate map data.
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    // Store map in 2d array.
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    rmapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            rmapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            if (num)
                TileMapGroup->AddNewObject(new Engine::Image("play/blue.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            else
                TileMapGroup->AddNewObject(new Engine::Image("play/red.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
}
void MODE2::ReadMonsterWave() {
    std::string filename = std::string("resources/enemy1.txt");
    // Read Monster file.
    float type, wait, repeat;
    MonsterWaveData.clear();
    std::ifstream fin(filename);
    while (fin >> type && fin >> wait && fin >> repeat) {
        for (int i = 0; i < repeat; i++)
            MonsterWaveData.emplace_back(type, wait);
    }
    fin.close();
}
void MODE2::ConstructUI() {
    // Background
    if(1) {
        UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
        // Text
        UIGroup->AddNewObject(
                new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
        UIGroup->AddNewObject(
                UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
        UIGroup->AddNewObject(
                UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));

        UIGroup->AddNewObject(
                Time = new Engine::Label(std::to_string(static_cast<int>(elapsedTime)) , "pirulen.ttf", 48, 1500, 50));

        // Buttons
        ConstructButton(0, "play/enemy-6.png", PlugGunHero::Price);
        AddNewObject(new Engine::Label("$40", "pirulen.ttf", 18, 1325, 210, 50, 200, 255, 255, 0.5, 0.5));
        ConstructButton(1, "play/enemy-1.png", L3Hero::Price);
        AddNewObject(new Engine::Label("$100", "pirulen.ttf", 18, 1400, 210, 100, 150, 255, 255, 0.5, 0.5));
        ConstructButton(2, "play/Dragon.png", L0Hero::Price);
        AddNewObject(new Engine::Label("$400", "pirulen.ttf", 18, 1475, 210, 255, 100, 255, 255, 0.5, 0.5));
        ConstructButton(3, "play/mario.png", L5Hero::Price);
        AddNewObject(new Engine::Label("$8000", "pirulen.ttf", 18, 1550, 210, 300, 50, 255, 255, 0.5, 0.5));

        ConstructButton(6, "play/enemy-2.png", LaserHero::Price);
        AddNewObject(new Engine::Label("$20", "pirulen.ttf", 18, 1325, 330, 300, 50, 255, 255, 0.5, 0.5));
        ConstructButton(7, "play/enemy-3.png", LaserHero::Price);
        AddNewObject(new Engine::Label("$300", "pirulen.ttf", 18, 1400, 330, 300, 50, 255, 255, 0.5, 0.5));
        ConstructButton(8, "play/enemy-7.png", LaserHero::Price);
        AddNewObject(new Engine::Label("$50", "pirulen.ttf", 18, 1475, 330, 300, 50, 255, 255, 0.5, 0.5));

        ConstructButton(10, "play/MoveTool.png", 0);
        AddNewObject(new Engine::Label("$0", "pirulen.ttf", 18, 1325, 450, 50, 200, 255, 255, 0.5, 0.5));
        ConstructButton(12, "play/PlusMoney.png", 0);
        AddNewObject(new Engine::Label("$0", "pirulen.ttf", 18, 1400, 450, 100, 150, 255, 255, 0.5, 0.5));
        ConstructButton(11, "play/Shovel.png", Shovel::Price);
        AddNewObject(new Engine::Label("$0", "pirulen.ttf", 18, 1475, 450, 255, 100, 255, 255, 0.5, 0.5));
        ConstructButton(13, "play/bullet-5.png", 1500);
        AddNewObject(new Engine::Label("$1500", "pirulen.ttf", 18, 1550, 450, 300, 50, 255, 255, 0.5, 0.5));
    }

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x , h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);
}

void MODE2::ConstructButton(int id, std::string sprite, int price) {
    HeroButton* btn;
    if (id <= 3) {
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png", 1294 + id * 76, 136, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + id * 76, 136 - 8, 0, 0, 0, 0)
                , 1294 + id * 76, 136, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 4){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png", 1294 + (id-4) * 76, 240, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + (id-4) * 76, 240 , 0, 0, 0, 0)
                , 1294 + (id-4) * 76, 136, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 5){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png", 1294 + (id-5) * 76, 240, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + (id-5) * 76, 240 , 0, 0, 0, 0)
                , 1294 + (id-5) * 76, 240, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 6){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png", 1294 + 76*0 , 240, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 , 240 , 0, 0, 0, 0)
                , 1294 , 240, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 7){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png", 1294 + 76 , 240, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294+76 , 240 , 0, 0, 0, 0)
                , 1294+76 , 240, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 8){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png", 1294 + 76*2 , 240, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294+76*2 , 240 , 0, 0, 0, 0)
                , 1294+76*2 , 240, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if (id == 10){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if (id == 11){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }else if (id == 13){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if (id == 12){
        btn = new HeroButton("play/floor.png", "play/dirt.png",
                             Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                             Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&MODE2::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
}

void MODE2::UIBtnClicked(int id) {
    const Engine::Point EndCoordinate = Engine::Point(EndGridPoint.x * BlockSize + BlockSize / 2,
                                                      EndGridPoint.y * BlockSize + BlockSize / 2);
    Hero *hero = nullptr;
    Monster* Monster = nullptr;
    if(1) {
        if (id == 0 && money >= PlugGunHero::Price) {
            HeroGroup->AddNewObject(hero = new PlugGunHero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-PlugGunHero::Price);
        }
        else if (id == 1 && money >= L3Hero::Price){
            HeroGroup->AddNewObject(hero = new L3Hero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-L3Hero::Price);
        }
        else if (id == 2 && money >= L0Hero::Price) {
            HeroGroup->AddNewObject(hero = new L0Hero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-L0Hero::Price);
        }
        else if (id == 3 && money >= L5Hero::Price){
            HeroGroup->AddNewObject(hero = new L5Hero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-L5Hero::Price);
        }
        else if (id == 6 && money >= LaserHero::Price) {
            HeroGroup->AddNewObject(hero = new LaserHero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-LaserHero::Price);
        }
        else if (id == 7 && money >= MissileHero::Price){
            HeroGroup->AddNewObject(hero = new MissileHero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-MissileHero::Price);
        }
        else if (id == 8 && money >= MachineGunHero::Price) {
            HeroGroup->AddNewObject(hero = new MachineGunHero(EndCoordinate.x, EndCoordinate.y));
            hero->UpdatePath(rmapDistance);
            hero->Update(ticks);
            EarnMoney(-MachineGunHero::Price);
        }
        else if (id == 12) {
            MODE2::EarnMoney(10);
            //UseTool = 3;
            return;
        } else if (id == 13) {
            if (money >= 1500) {
                MODE2::EarnMoney(-1500);
                UILives->Text = std::string("Life ") + std::to_string(++lives);
                //UseTool = 4;
            } else {

            }
            return;
        }
        if (!preview)
            return;
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        preview->Tint = al_map_rgba(255, 255, 255, 200);
        //preview->Enabled = false;
        //preview->Preview = true;
        UIGroup->AddNewObject(preview);
        OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x,
                    Engine::GameEngine::GetInstance().GetMousePosition().y);
        /*
        if (hero) {
            HeroGroup->AddNewObject(hero);
            //hero->Update(0);
            hero->UpdatePath(rmapDistance);
            // Compensate the time lost.
            hero->Update(ticks);
            hero = nullptr;
            //OnMouseMove(mx,my);
        }*/
    }
}

std::vector<std::vector<int>> MODE2::CalculateBFSDistance() {
    // Reverse BFS to find path.
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
    std::queue<Engine::Point> que;
    // Push end point.
    // BFS from end point.
    if (mapState[MapHeight - 1][MapWidth - 1] != 0)
        return map;
    que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
    map[MapHeight - 1][MapWidth - 1] = 0;
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();
        for (auto &c : directions) {
            int x = p.x + c.x;
            int y = p.y + c.y;
            if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight ||
                map[y][x] != -1 || mapState[y][x] != 0) {
                continue;
            } else {
                map[y][x] = map[p.y][p.x] + 1;
                que.push(Engine::Point(x, y));
            }
        }
    }
    return map;
}

std::vector<std::vector<int>> MODE2::CalculateDistance() {
    // Reverse BFS to find path.
    std::vector<std::vector<int>> rmap(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
    std::queue<Engine::Point> rque;
    // Push end point.
    // BFS from end point.
    if (rmapState[MapHeight - 1][MapWidth - 1] != 0)
        return rmap;
    rque.push(Engine::Point(MapWidth - 1, MapHeight - 1));
    rmap[MapHeight - 1][MapWidth - 1] = 0;
    while (!rque.empty()) {
        Engine::Point p = rque.front();
        rque.pop();
        for (auto &c : directions) {
            int x = p.x + c.x;
            int y = p.y + c.y;
            if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight ||
                rmap[y][x] != -1 || rmapState[y][x] != 0) {
                continue;
            } else {
                rmap[y][x] = rmap[p.y][p.x] + 1;
                rque.push(Engine::Point(x, y));
            }
        }
    }
    // 反转距离，计算从每个节点到终点的距离。
    int maxDistance = 0;
    for (int i = 0; i < MapHeight; ++i) {
        for (int j = 0; j < MapWidth; ++j) {
            if (rmap[i][j] > maxDistance) {
                maxDistance = rmap[i][j];
            }
        }
    }

    for (int i = 0; i < MapHeight; ++i) {
        for (int j = 0; j < MapWidth; ++j) {
            if (rmap[i][j] != -1) {
                rmap[i][j] = maxDistance - rmap[i][j];
            }
        }
    }
    return rmap;
}



float MODE2::GetElapsedTime() const {
    return elapsedTime;
}

