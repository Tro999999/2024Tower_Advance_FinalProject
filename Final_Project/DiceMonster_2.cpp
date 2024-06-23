#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "DiceMonster_2.hpp"
#include"DiceMonster.hpp"
#include "mode2.hpp"
#include "Hero.hpp"
#include "AudioHelper.hpp"
#include "Weapon.hpp"
#include "DirtyEffect2.hpp"
#include "Monster.hpp"
#include "ExplosionEffect2.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "LOG.hpp"


DiceMonster_2::DiceMonster_2(int x, int y) : Monster("play/dice-2.png", x, y, 25, 45, 4000, 100) {
}

void DiceMonster_2::OnExplode() {
    getMode2Scene()->EffectGroup->AddNewObject(new ExplosionEffect2(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);

    // Compensate the time lost.

    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getMode2Scene()->GroundEffectGroup->AddNewObject(new DirtyEffect2("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
    Monster* monster;
    getMode2Scene()->MonsterGroup->AddNewObject(monster = new DiceMonster(Position.x, Position.y));
    monster->UpdatePath(getMode2Scene()->mapDistance);
    monster->Update(0);
}

