#include <string>

#include "BossEnemy.hpp"
#include "Enemy.hpp"
#include "PlayScene.hpp"

BossEnemy::BossEnemy(int x, int y) : Enemy("play/Ghost.png", x, y, 25, 30, 50000, 500) {
}

