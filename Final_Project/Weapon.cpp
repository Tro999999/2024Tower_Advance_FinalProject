#include "Weapon.hpp"
#include "Collider.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "IScene.hpp"
#include "mode2.hpp"
#include "Point.hpp"
#include "Sprite.hpp"
#include "Turret.hpp"
#include "Monster.hpp"
#include "Hero.hpp"
#include "L5Turret.hpp"
#include "mode2.hpp"


MODE2* Weapon::getMode2Scene() {
    return dynamic_cast<MODE2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}



Weapon::Weapon(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Hero* parent) :
        Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
    Velocity = forwardDirection.Normalize() * speed;
    Rotation = rotation;
    CollisionRadius = 4;
}

void Weapon::OnExplode(Monster* monster) {
}
void Weapon::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    if(damage == 1500){
        if (parent) {
            Rotation = Rotation + deltaTime ;
            float radius = 200 ,angle = Rotation;
            float x = parent->Position.x + parent->Size.x / 2 + cos(angle) * radius, y =parent->Position.y + parent->Size.y / 2 - sin(angle) * radius;
            Position = Engine::Point(x - Size.x / 2, y - Size.y / 2);
        }
    }
    MODE2* scene = getMode2Scene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    for (auto& it : scene->MonsterGroup->GetObjects()) {
        Monster* monster = dynamic_cast<Monster*>(it);
        if (!monster->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, monster->Position, monster->CollisionRadius)) {
            //OnExplode(enemy);
            monster->Hit(damage);
            getMode2Scene()->WeaponGroup->RemoveObject(objectIterator);
            monster->IsBoss();

            monster->IsBoss();
            break;
        }
    }
    // Check if out of boundary.
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), MODE2::GetClientSize()))
        if(damage != 90){

        }
        else {
            getMode2Scene()->WeaponGroup->RemoveObject(objectIterator);
        }
}
