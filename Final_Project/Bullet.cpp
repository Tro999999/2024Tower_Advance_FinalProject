#include "Bullet.hpp"
#include "Collider.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Sprite.hpp"
#include "Turret.hpp"
#include "L5Turret.hpp"


PlayScene* Bullet::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}



Bullet::Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent) :
	Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
	Velocity = forwardDirection.Normalize() * speed;
	Rotation = rotation;
	CollisionRadius = 4;
}

void Bullet::OnExplode(Enemy* enemy) {
}
void Bullet::Update(float deltaTime) {
	Sprite::Update(deltaTime);
    if(damage == 1500){
        if (parent) {
            Rotation = Rotation + deltaTime ;
            float radius = 200 ,angle = Rotation;
            float x = parent->Position.x + parent->Size.x / 2 + cos(angle) * radius, y =parent->Position.y + parent->Size.y / 2 - sin(angle) * radius;
            Position = Engine::Point(x - Size.x / 2, y - Size.y / 2);
        }
    }
	PlayScene* scene = getPlayScene();
	// Can be improved by Spatial Hash, Quad Tree, ...
	// However simply loop through all enemies is enough for this program.
	for (auto& it : scene->EnemyGroup->GetObjects()) {
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		if (!enemy->Visible)
			continue;
		if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
			//OnExplode(enemy);
			enemy->Hit(damage);
			getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            enemy->IsBoss();

            if (this->parent != NULL && this->parent->price == 8000) {
                this->parent->CountBullet--;
            }
            enemy->IsBoss();
            break;
		}
	}
	// Check if out of boundary.
	if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
		if(damage != 90){

        }
        else {
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        }
}
