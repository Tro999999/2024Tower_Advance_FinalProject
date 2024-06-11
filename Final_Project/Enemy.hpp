#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "Sprite.hpp"

class Bullet;
class PlayScene;
class Turret;

class Enemy : public Engine::Sprite {
protected:
 	std::vector<Engine::Point> path;
	float speed;
	float maxSpeed;
	float hp;
	float slowRemainTime;
	int money;
	PlayScene* getPlayScene();
public:
	float reachEndTime;
    float gentime = 0.0;
    float ticks;
	std::list<Turret*> lockedTurrets;
	std::list<Bullet*> lockedBullets;
    std::vector<std::vector<int>> mapDistance;
	Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money);
 	void Hit(float damage);
	void UpdatePath(const std::vector<std::vector<int>>& mapDistance);
	void Update(float deltaTime) override;
	void Draw() const override;
	void Slow(float slowFactor, float slowTime);
	void SlowEffect(float deltaTime);
    void IsBoss();
    virtual void OnExplode();
};
#endif // ENEMY_HPP
