#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "DiceEnemy_2.hpp"
#include"DiceEnemy.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"
#include "AudioHelper.hpp"
#include "Bullet.hpp"
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "ExplosionEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"


DiceEnemy_2::DiceEnemy_2(int x, int y) : Enemy("play/dice-2.png", x, y, 25, 45, 4000, 100) {
}

void DiceEnemy_2::OnExplode() {
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);

    // Compensate the time lost.

    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
    Enemy* enemy;
    getPlayScene()->EnemyGroup->AddNewObject(enemy = new DiceEnemy(Position.x, Position.y));
    enemy->UpdatePath(getPlayScene()->mapDistance);
    enemy->Update(0);
}

