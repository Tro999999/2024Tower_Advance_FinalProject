#ifndef MAP_EDITOR_SCENE_HPP
#define MAP_EDITOR_SCENE_HPP

#include "IScene.hpp"
#include "Point.hpp"
#include <vector>
#include <string>

class MapEditorScene final : public Engine::IScene {
public:
    explicit MapEditorScene() = default;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;

private:
    std::vector<std::vector<int>> mapData;
    int MapWidth = 20;
    int MapHeight = 13;
    int BlockSize = 64;
    std::string mapFile = "resources/map4.txt";
    void LoadMap();
    void SaveMap();
    void ToggleBlock(int x, int y);
};

#endif // MAP_EDITOR_SCENE_HPP
