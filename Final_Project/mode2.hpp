#ifndef MODE2_HPP
#define MODE2_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <string>

#include "IScene.hpp"
#include "Point.hpp"


class Hero;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}  // namespace Engine

class MODE2 final : public Engine::IScene {
private:
    enum TileType {
        TILE_DIRT,
        TILE_FLOOR,
        TILE_OCCUPIED,
    };
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
protected:
    int lives;
    int money;
    int SpeedMult;
    bool isStage1;
    float startTime;
    float elapsedTime;
    float lastUpdateTime;
public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const float DangerTime;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    bool mute;
    bool isTool;
    int MapId;
    float ticks;
    float deathCountDown;
    // Map tiles.
    Group* TileMapGroup;
    Group* GroundEffectGroup;
    Group* DebugIndicatorGroup;
    Group* ToolGroup;
    Group* TowerGroup;
    Group* HeroGroup;
    Group* MonsterGroup;
    Group* WeaponGroup;
    Group* EffectGroup;
    Group* UIGroup;
    Engine::Label* UIMoney;
    Engine::Label* Time;
    Engine::Label* UILives;
    Engine::Image* imgTarget;
    Engine::Sprite* dangerIndicator;
    Hero* preview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> MonsterWaveData;
    std::list<int> keyStrokes;
    static Engine::Point GetClientSize();
    explicit MODE2() = default;
    void Initialize() override;
    void Terminate() override;
    void StartTimer();
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit();
    void Attack();
    int GetMoney() const;
    void EarnMoney(int money);
    void ReadMap();
    void ReadMonsterWave();
    void ConstructUI();
    void ConstructButton(int id, std::string sprite, int price);
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    float GetElapsedTime() const; // Add this method
    std::vector<std::vector<int>> CalculateBFSDistance();
    // void ModifyReadMapTiles();
};
#endif
