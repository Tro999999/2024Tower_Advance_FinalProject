#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "DiceEnemy.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"
#include "AudioHelper.hpp"
#include "Bullet.hpp"
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "ExplosionEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "DiceEnemy_2.hpp"
#include "IScene.hpp"
#include "LOG.hpp"

DiceEnemy::DiceEnemy(int x, int y) : Enemy("play/dice-1.png", x, y, 25, 70, 3000, 50) {
    //數字為1的骰子 由２死後生出來
}

