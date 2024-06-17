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
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "PlayScene.hpp"
// Turret
#include "PlugGunTurret.hpp"
#include "L0Turret.hpp"
#include "L2Turret.hpp"
#include "L3Turret.hpp"
#include "L4Turret.hpp"
#include "L5Turret.hpp"
#include "LaserTurret.hpp"
#include "MachineGunTurret.hpp"
#include "MissileTurret.hpp"
#include "Plane.hpp"

#include "SettingScene.hpp"
#include "ScoreboardScene.hpp"
#include "MoveTurret.hpp"

//Tool
#include "MoveTool.hpp"
#include "Shovel.hpp"
// Enemy
#include "Enemy.hpp"
#include "PoopEnemy.hpp"
#include "DevilEnemy.hpp"
#include "wolfknight.hpp"
#include "RedNormalEnemy.hpp"
#include "BossEnemy.hpp"
#include "DiceEnemy.hpp"
#include "DiceEnemy_2.hpp"
#include "BlueNormalEnemy.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "Sprite.hpp"
#include "Bullet.hpp"
#include "Turret.hpp"
#include "TurretButton.hpp"
#include "LOG.hpp"
#include "Collider.hpp"
#include "Point.hpp"
#include "Tool.hpp"
#include "ToolButton.hpp"

int check = 0;


bool PlayScene::DebugMode = false;
int UseTool = 0;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
float val =  AudioHelper::SFXVolume;


//const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP };
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_Z, ALLEGRO_KEY_X, ALLEGRO_KEY_C,ALLEGRO_KEY_V, ALLEGRO_KEY_B, ALLEGRO_KEY_N };
Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
	isTool = false;
    mapState.clear();
	keyStrokes.clear();
	ticks = 0;
	deathCountDown = -1;
	lives = 10;
	money = 150;
	SpeedMult = 1;
    elapsedTime = 0;
    lastUpdateTime = 0;
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
    AddNewObject(ToolGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();

    if(MapId == 1) check = 1, isStage1 = true;
    startTime = al_get_time();

	ReadEnemyWave();
	mapDistance = CalculateBFSDistance();
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
    //Engine::ImageButton* btn;
    //btn = new Engine::ImageButton("start_scene/out.png", "start_scene/in.png", halfW * 1.65, halfH*1.6, 250, 67.5);
    //沒加上面這行會跳出畫面
    //AudioHelper::StopSample(bgmInstance);
    //AddNewControlObject(btn);
    AddNewObject(new Engine::Label("<esc to exit>", "pirulen.ttf", 25, halfW * 1.7 + 75 , halfH * 1.6 + 40 , 0, 0, 0, 255, 0.5, 0.5));

    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
	if (!mute)
        if(scene->MapId == 2){
            bgmInstance = AudioHelper::PlaySample("play2.ogg", true, AudioHelper::BGMVolume);
        }
        else{bgmInstance = AudioHelper::PlaySample("play.ogg", true, AudioHelper::BGMVolume);}
    else
        bgmInstance = AudioHelper::PlaySample("play.ogg", true, 0.0);
}
void PlayScene::Terminate() {
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	AudioHelper::StopSample(deathBGMInstance);
	deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
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
	for (auto& it : EnemyGroup->GetObjects()) {
		reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
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
    else{

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
            if (enemyWaveData.empty()) {
                if (EnemyGroup->GetObjects().empty()) {
                    //delete TileMapGroup;
                    //delete GroundEffectGroup;
                    //delete DebugIndicatorGroup;
                    //delete TowerGroup;
                    //delete EnemyGroup;
                    //delete BulletGroup;
                    //delete EffectGroup;
                    //delete UIGroup;
                    //delete imgTarget;
                    if(check){
                        MapId = 2;
                        check = 0;
                        AudioHelper::ChangeSampleVolume(bgmInstance, 0.0);
                        Engine::GameEngine::GetInstance().ChangeScene("play");
                    }
                    else Engine::GameEngine::GetInstance().ChangeScene("win");
                }
                continue;
            }


            auto current = enemyWaveData.front();
            if (ticks < current.second)
                continue;
            ticks -= current.second;
            enemyWaveData.pop_front();
            const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2,
                                                                SpawnGridPoint.y * BlockSize + BlockSize / 2);
            Enemy *enemy;
            switch (current.first) {
                case 1:
                    EnemyGroup->AddNewObject(enemy = new RedNormalEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 2:
                    //std::cout << "enemy : 2\n" ;
                    EnemyGroup->AddNewObject(enemy = new BlueNormalEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 3:
                    EnemyGroup->AddNewObject(enemy = new DiceEnemy_2(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 4:
                    EnemyGroup->AddNewObject(enemy = new BossEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                    //case 5 6
                case 7:
                    EnemyGroup->AddNewObject(enemy = new DevilEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 5:
                    EnemyGroup->AddNewObject(enemy = new PoopEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 6:
                    EnemyGroup->AddNewObject(enemy = new DiceEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                case 8:
                    EnemyGroup->AddNewObject(enemy = new wolfknight(SpawnCoordinate.x, SpawnCoordinate.y));
                    break;
                default:
                    continue;
            }
            enemy->UpdatePath(mapDistance);
            // Compensate the time lost.
            enemy->Update(ticks);
        }
    }
    else{

    }
	if (preview) {
		preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
		// To keep responding when paused.
        /*preview->Update(deltaTime);*/
		preview->Update(deltaTime);
	}
}
void PlayScene::Draw() const {
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
    else{

    }
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
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
void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize, y = my / BlockSize ;
	if (button & 1) {
		if ( preview->GetPrice()!= 0 && mapState[y][x] != 2 ) {
			if (!preview)
				return;
			// Check if valid.
			if (CheckSpaceValid(x, y)) {

			}
            else{
                Engine::Sprite* sprite;
                GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
                sprite->Rotation = 0;
                return;
            }
            if(preview->price == 0){
                return ;
            }
			// Purchase.
            if(UseTool == 1){
            }
            else {
                EarnMoney(-preview->GetPrice());
            }
            UseTool = 0;
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
            preview->Position.x = x * BlockSize + BlockSize / 2;
            preview->Position.y = y * BlockSize + BlockSize / 2;
            preview->Enabled = true;
            preview->Preview = false;
            preview->Tint = al_map_rgba(255, 255, 255, 255);
            TowerGroup->AddNewObject(preview);

            if (preview->GetPrice() == 200) {
                mapState[y][x] = LevelUpTurret;
            }
            else {
                mapState[y][x] = TILE_OCCUPIED;
            }
            preview->Update(0);
            preview->CountBullet = 0;
            preview = nullptr;
            OnMouseMove(mx, my);
            return;
		}
        else if (UseTool == 2 && (mapState[y][x] != 0 && mapState[y][x] != 1) && preview->GetPrice() == 0 ) {
            if (!preview)
                return;

            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());

            std::cout << "mx,my : " << mx << " " << my << '\n';
            float mx2 = 64 * (mx/64 + 0.5);
            float my2 = 64 * (my/64 + 0.5);
            Engine::Point p(mx2,my2);
            for (auto& it : TowerGroup->GetObjects()) {
                Turret* turret = dynamic_cast<Turret*>(it);

                if (Engine::Collider::IsCircleOverlap(p, 10, turret->Position, 10)) {
                    for (auto& it : BulletGroup->GetObjects()) {
                        Bullet* bullet = dynamic_cast<Bullet*>(it);
                        if (bullet->parent == turret) {
                            BulletGroup->RemoveObject(bullet->GetObjectIterator());
                        }
                    }
                    TowerGroup->RemoveObject(turret->GetObjectIterator());
                    EarnMoney(turret->GetPrice()/2);
                    break;
                }
            }

            UseTool = 0;
            preview = nullptr;
            mapState[y][x] = TILE_FLOOR;
            //std::cout << "In OnMouseUp : " <<mapState[y][x] << "\n";
            OnMouseMove(mx, my);
            return;
        }
        else if (mapState[y][x] == 2 && preview->GetPrice() == 100){
            if (!preview)
                return;
            // Check if valid.
            //preview = new L2Turret(0,0);
            if (!CheckSpaceValid(x, y) || preview->price == 0) {
                return;
            }

            float mx2 = 64 * (mx/64 + 0.5) , my2 = 64 * (my/64 + 0.5);
            Engine::Point p(mx2,my2);
            for (auto& it : TowerGroup->GetObjects()) {
                Turret* turret = dynamic_cast<Turret*>(it);
                if (Engine::Collider::IsCircleOverlap(p, 10, turret->Position, 10)) {
                    if (turret->GetPrice() != 100) {
                        return;
                    }else {

                    }
                    TowerGroup->RemoveObject(turret->GetObjectIterator());
                    break;
                }
            }
            // Purchase.
            if (!UseTool) {
                EarnMoney(-preview->GetPrice());
            }
            else {
                UseTool = 0;
            }
            UseTool = 0;
            // Remove Preview.
            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            // Construct real turret.

            preview->Position.x = x * BlockSize + BlockSize / 2;
            preview->Position.y = y * BlockSize + BlockSize / 2;
            preview->Enabled = true;
            preview->Preview = false;
            preview->Tint = al_map_rgba(255, 255, 255, 255);
            //TowerGroup->AddNewObject(new L3Turret(preview->Position.x,preview->Position.y));
            //TowerGroup->RemoveObject(preview->GetObjectIterator());  //加了會crash掉
            //TowerGroup->RemoveObject(objectIterator);
            Turret* temp;
            TowerGroup->AddNewObject(temp = new L4Turret(preview->Position.x,preview->Position.y));
            temp->Update(0);
            preview = nullptr;
            mapState[y][x] = LevelUpTurret;

            OnMouseMove(mx, my);
            return;
        }
        else if (UseTool == 1 && preview->GetPrice() == 0 &&  (mapState[y][x] != 0 && mapState[y][x] != 1)) {
            if (!preview)
                return;

            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            // Construct real turret.
            std::cout << "mx,my : " << mx << " " << my << '\n';
            float mx2 = 64 * (mx/64 + 0.5);
            float my2 = 64 * (my/64 + 0.5);
            Engine::Point p(mx2,my2);
            for (auto& it : TowerGroup->GetObjects()) {
                Turret* turret = dynamic_cast<Turret*>(it);
                if (Engine::Collider::IsCircleOverlap(p, 10, turret->Position, 10)) {
                    if (turret->GetPrice() == 40) {
                        preview = new PlugGunTurret(0,0);
                    }
                    else if (turret->GetPrice() == 60) {
                        preview = new L2Turret(0,0);
                    }
                    else if (turret->GetPrice() == 400) {
                        preview = new L0Turret(0,0);
                    }
                    else if (turret->GetPrice() == 100) {
                        preview = new L3Turret(0,0);
                    }
                    else if (turret->GetPrice() == 8000) {
                        preview = new L5Turret(0,0);
                    }
                    else if (turret->GetPrice() == 200) {
                        preview = new L4Turret(0,0);
                    }
                    else if (turret->GetPrice() == 20) {
                        preview = new LaserTurret(0,0);
                    }
                    else if (turret->GetPrice() == 300) {
                        preview = new MissileTurret(0,0);
                    }
                    else if (turret->GetPrice() == 50) {
                        preview = new MachineGunTurret(0,0);
                    }

                    for (auto& it : BulletGroup->GetObjects()) {
                        Bullet* bullet = dynamic_cast<Bullet*>(it);
                        if (bullet->parent == turret) {
                            BulletGroup->RemoveObject(bullet->GetObjectIterator());
                        }
                    }
                    TowerGroup->RemoveObject(turret->GetObjectIterator());
                    preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
                    preview->Tint = al_map_rgba(255, 255, 255, 255);
                    preview->Enabled = false;
                    preview->Preview = true;
                    UIGroup->AddNewObject(preview);
                    OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
                    break;
                }
            }
            mapState[y][x] = TILE_FLOOR;
            return;
        }
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode == ALLEGRO_KEY_TAB) {
        PlayScene::DebugMode ^= 1;

	}

	else {
        keyStrokes.push_back(keyCode);
        if (keyStrokes.size() > code.size())
            keyStrokes.pop_front();
        // Active a plane : EffectGroup->AddNewObject(new Plane());
        // Earn money : money += 10000;
        std::vector<int> compare;
        for (auto &i: keyStrokes) compare.push_back((i));
        if (compare == PlayScene::code){
            EffectGroup->AddNewObject(new Plane());
            money =  money + 10000;
        }
	}
	if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for PlugGunTurret.
		UIBtnClicked(0);
	}
	else if (keyCode == ALLEGRO_KEY_W) {
		// Hotkey for new turret.
        UIBtnClicked(1);
	}
    else if (keyCode == ALLEGRO_KEY_A) {
        // Hotkey for new turret.
        UIBtnClicked(10);
    }
    else if (keyCode == ALLEGRO_KEY_E) {
        // Hotkey for new turret.
        UIBtnClicked(2);
    }
    else if (keyCode == ALLEGRO_KEY_T) {
        // Hotkey for new turret.
        UIBtnClicked(4);
    }
    else if (keyCode == ALLEGRO_KEY_S) {
        // Hotkey for new turret.
        UIBtnClicked(11);
    }
    else if (keyCode == ALLEGRO_KEY_R) {
        // Hotkey for new turret.
        UIBtnClicked(3);
    }
    else if (keyCode == ALLEGRO_KEY_D) {
        // Hotkey for new turret.
        UIBtnClicked(12);
    }
	else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		SpeedMult = keyCode - ALLEGRO_KEY_0;
	}
	else if (keyCode == ALLEGRO_KEY_M) {
		// Hotkey for mute / unmute.
        if (mute) {
            AudioHelper::SFXVolume = val;
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
        Engine::GameEngine::GetInstance().ChangeScene("mode1");
    }
    else if(keyCode == ALLEGRO_KEY_UP){

    }
    else if(keyCode == ALLEGRO_KEY_DOWN){

    }
    else if(keyCode == ALLEGRO_KEY_LEFT){

    }
    else if(keyCode == ALLEGRO_KEY_RIGHT){

    }
}
void PlayScene::Hit() {
	UILives->Text = std::string("Life ") + std::to_string(--lives);
	if (lives <= 0) {
		Engine::GameEngine::GetInstance().ChangeScene("lose");
	}
}
int PlayScene::GetMoney() const {
	return money;
}
void PlayScene::EarnMoney(int money) {
	this->money += money;
	UIMoney->Text = std::string("$") + std::to_string(this->money);
}



void PlayScene::ReadMap() {
	std::string filename = std::string("resources/map") + std::to_string(MapId) + ".txt";
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
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			if (num)
				TileMapGroup->AddNewObject(new Engine::Image("play/blue.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			else
				TileMapGroup->AddNewObject(new Engine::Image("play/red.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
		}
	}
}
void PlayScene::ReadEnemyWave() {
	std::string filename = std::string("resources/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	float type, wait, repeat;
	enemyWaveData.clear();
	std::ifstream fin(filename);
	while (fin >> type && fin >> wait && fin >> repeat) {
		for (int i = 0; i < repeat; i++)
			enemyWaveData.emplace_back(type, wait);
	}
	fin.close();
}
void PlayScene::ConstructUI() {
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
        ConstructButton(0, "play/turret-6.png", PlugGunTurret::Price);
        AddNewObject(new Engine::Label("$40", "pirulen.ttf", 18, 1325, 210, 50, 200, 255, 255, 0.5, 0.5));
        ConstructButton(1, "play/turret-1.png", L3Turret::Price);
        AddNewObject(new Engine::Label("$100", "pirulen.ttf", 18, 1400, 210, 100, 150, 255, 255, 0.5, 0.5));
        ConstructButton(2, "play/Dragon.png", L0Turret::Price);
        AddNewObject(new Engine::Label("$400", "pirulen.ttf", 18, 1475, 210, 255, 100, 255, 255, 0.5, 0.5));
        ConstructButton(3, "play/mario.png", L5Turret::Price);
        AddNewObject(new Engine::Label("$8000", "pirulen.ttf", 18, 1550, 210, 300, 50, 255, 255, 0.5, 0.5));

        ConstructButton(6, "play/turret-2.png", LaserTurret::Price);
        AddNewObject(new Engine::Label("$20", "pirulen.ttf", 18, 1325, 330, 300, 50, 255, 255, 0.5, 0.5));
        ConstructButton(7, "play/turret-3.png", LaserTurret::Price);
        AddNewObject(new Engine::Label("$300", "pirulen.ttf", 18, 1400, 330, 300, 50, 255, 255, 0.5, 0.5));
        ConstructButton(8, "play/turret-7.png", LaserTurret::Price);
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
    else{

    }

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x , h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int shift = 135 + 25;
	dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
	dangerIndicator->Tint.a = 0;
	UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::ConstructButton(int id, std::string sprite, int price) {
	TurretButton* btn;
    if (id <= 3) {
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png", 1294 + id * 76, 136, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + id * 76, 136 - 8, 0, 0, 0, 0)
                , 1294 + id * 76, 136, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 4){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png", 1294 + (id-4) * 76, 240, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + (id-4) * 76, 240 , 0, 0, 0, 0)
                , 1294 + (id-4) * 76, 136, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 5){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png", 1294 + (id-5) * 76, 240, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + (id-5) * 76, 240 , 0, 0, 0, 0)
                , 1294 + (id-5) * 76, 240, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 6){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png", 1294 + 76*0 , 240, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 , 240 , 0, 0, 0, 0)
                , 1294 , 240, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 7){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png", 1294 + 76 , 240, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294+76 , 240 , 0, 0, 0, 0)
                , 1294+76 , 240, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if(id == 8){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png", 1294 + 76*2 , 240, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294+76*2 , 240 , 0, 0, 0, 0)
                , 1294+76*2 , 240, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if (id == 10){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if (id == 11){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }else if (id == 13){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
    else if (id == 12){
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/tower-base.png",1294 + (id-10) * 76, 250+114, 0, 0, 0, 0),
                               Engine::Sprite(sprite, 1294 + (id-10) * 76, 250+114, 0, 0, 0, 0)
                , 1294 + (id-10) * 76, 250+114, price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
        UIGroup->AddNewControlObject(btn);
    }
}

void PlayScene::UIBtnClicked(int id) {
    if(1) {
        if (preview) {
            UIGroup->RemoveObject(preview->GetObjectIterator());
            preview = nullptr;
        } else if (id == 0 && money >= PlugGunTurret::Price)
            preview = new PlugGunTurret(0, 0);
        else if (id == 1 && money >= L2Turret::Price)
            preview = new L3Turret(0, 0);
        else if (id == 2 && money >= L0Turret::Price)
            preview = new L0Turret(0, 0);
        else if (id == 3 && money >= L5Turret::Price)
            preview = new L5Turret(0, 0);

        else if (id == 6 && money >= LaserTurret::Price)
            preview = new LaserTurret(0, 0);
        else if (id == 7 && money >= MissileTurret::Price)
            preview = new MissileTurret(0, 0);
        else if (id == 8 && money >= MachineGunTurret::Price)
            preview = new MachineGunTurret(0, 0);

        else if (id == 12) {
            PlayScene::EarnMoney(10);
            //UseTool = 3;
            return;
        } else if (id == 13) {
            if (money >= 1500) {
                EarnMoney(-1500);
                UILives->Text = std::string("Life ") + std::to_string(++lives);
                //UseTool = 4;
            } else {

            }
            return;
        } else if (id == 10) {
            preview = new MoveTool(0, 0);
            UseTool = 1;
        } else if (id == 11) {
            preview = new Shovel(0, 0);
            UseTool = 2;
        }
        if (!preview)
            return;
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        preview->Tint = al_map_rgba(255, 255, 255, 200);
        preview->Enabled = false;
        preview->Preview = true;
        UIGroup->AddNewObject(preview);
        OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x,
                    Engine::GameEngine::GetInstance().GetMousePosition().y);
    }
    else{

    }
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
    if (mapState[y][x] == LevelUpTurret) {
        return false;
    }

	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	if (map[0][0] == -1)
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0) pnt.x = 0;
		if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
		if (pnt.y < 0) pnt.y = 0;
		if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
		if (map[pnt.y][pnt.x] == -1)
			return false;
	}
	// All enemy have path to exit.
	mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
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

float PlayScene::GetElapsedTime() const {
    return elapsedTime;
}

