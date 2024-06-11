#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "WoodBullet.hpp"
#include "DoubleBullet.hpp"
#include "GoldenBullet.hpp"
#include "Group.hpp"
#include "SmallRocket.hpp"
#include "L5Turret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "ShootEffect.hpp"

const int L5Turret::Price = 8000;
L5Turret::L5Turret(float x, float y) :
        Turret("play/tower-base.png", "play/mario.png", x, y, 250, Price, 0) {
    // Move center downward, since we the turret head is slightly biased upward
    Anchor.y += 8.0f / GetBitmapHeight();
}

void L5Turret::CreateBullet() {



    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point perpendicular = Engine::Point(-normalized.y, normalized.x);

    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet(Position + normalized * 36, diff, rotation, this));
    getPlayScene()->EffectGroup->AddNewObject(new ShootEffect(Position.x + normalized.x * 36 , Position.y + normalized.y * 36));
    AudioHelper::PlayAudio("gun.wav");

    if (CountBullet == 0 && i <= 1) {
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized * 36, diff - diff, 0.0, this));
        //getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized * 36 * 4, diff - diff, 0.0, this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular * 36, diff- diff, ALLEGRO_PI / 2 , this));
        //getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular * 36 * 4, diff- diff, ALLEGRO_PI / 2 , this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized* -1 * 36, diff- diff, ALLEGRO_PI  , this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular* -1 * 36, diff- diff, ALLEGRO_PI * 1.5, this));

        CountBullet = 2;
        i++;
    }
    else if (CountBullet == 0 && i <= 2) {

        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized * 36, diff - diff, 0.0, this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular * 36, diff- diff, ALLEGRO_PI / 2 , this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized* -1 * 36, diff- diff, ALLEGRO_PI  , this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular* -1 * 36, diff- diff, ALLEGRO_PI * 1.5, this));

        CountBullet = 2;
        i = 3;
    }
    else if (CountBullet == 0) {
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized * 36, diff - diff, 0.0, this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular * 36, diff- diff, ALLEGRO_PI / 2 , this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + normalized* -1 * 36, diff- diff, ALLEGRO_PI  , this));
        getPlayScene()->BulletGroup->AddNewObject(new DoubleBullet2(Position + perpendicular* -1 * 36, diff- diff, ALLEGRO_PI * 1.5, this));

        CountBullet = 2;
    }

}
