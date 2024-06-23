
#include "GameEngine.hpp"
#include "LOG.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "StartScene.hpp"
#include "StageSelectScene.hpp"
#include "WinScene.hpp"
#include "SettingScene.hpp"
#include "ScoreboardScene.hpp"
#include "GameSelectScene.hpp"
#include "mode2.hpp"
#include "mode3.hpp"
#include "MapEditorScene.hpp"
#include "StageSelect2.hpp"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("mode1", new StageSelectScene());
    game.AddNewScene("mode2", new MODE2());
    game.AddNewScene("mode3", new Mode3());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
    game.AddNewScene("map-editor", new MapEditorScene());
	game.AddNewScene("win", new WinScene());
    game.AddNewScene("start-scene", new StartScene());
    game.AddNewScene("setting", new SettingScene());
    game.AddNewScene("game-select", new GameSelectScene());
    game.AddNewScene("stage-select", new StageSelect2());
    game.AddNewScene("scoreboard", new ScoreboardScene());
	game.Start("start-scene", 60, 1600, 832);
	return 0;
}
