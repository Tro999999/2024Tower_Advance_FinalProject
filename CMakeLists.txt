cmake_minimum_required(VERSION 3.25.2)
project(TowerDefense)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall")

find_package(PkgConfig)

pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)

file(COPY Final_Project/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TowerDefense Final_Project/main.cpp
        Final_Project/Allegro5Exception.cpp Final_Project/Allegro5Exception.hpp
        Final_Project/AudioHelper.cpp Final_Project/AudioHelper.hpp
        Final_Project/GameEngine.cpp Final_Project/GameEngine.hpp
        Final_Project/Group.cpp Final_Project/Group.hpp
        Final_Project/IObject.cpp Final_Project/IObject.hpp
        Final_Project/IScene.cpp Final_Project/IScene.hpp
        Final_Project/Image.cpp Final_Project/Image.hpp
        Final_Project/IControl.cpp Final_Project/IControl.hpp
        Final_Project/Enemy.cpp Final_Project/Enemy.hpp
        Final_Project/Turret.cpp Final_Project/Turret.hpp
        Final_Project/Label.cpp Final_Project/Label.hpp
        Final_Project/LOG.cpp Final_Project/LOG.hpp
        Final_Project/Bullet.cpp Final_Project/Bullet.hpp
        Final_Project/Collider.cpp Final_Project/Collider.hpp
        Final_Project/DirtyEffect.cpp Final_Project/DirtyEffect.hpp
        Final_Project/ExplosionEffect.cpp Final_Project/ExplosionEffect.hpp
        Final_Project/ImageButton.cpp Final_Project/ImageButton.hpp
        Final_Project/Plane.cpp Final_Project/Plane.hpp
        Final_Project/PlayScene.cpp Final_Project/PlayScene.hpp
        Final_Project/LoseScene.cpp Final_Project/LoseScene.hpp
        Final_Project/Resources.cpp Final_Project/Resources.hpp
        Final_Project/Sprite.cpp Final_Project/Sprite.hpp
        Final_Project/StartScene.cpp Final_Project/StartScene.hpp
        Final_Project/StageSelectScene.cpp Final_Project/StageSelectScene.hpp
        Final_Project/PlugGunTurret.cpp Final_Project/PlugGunTurret.hpp
        Final_Project/Point.cpp Final_Project/Point.hpp
        Final_Project/RedNormalEnemy.cpp Final_Project/RedNormalEnemy.hpp
        Final_Project/Slider.cpp Final_Project/Slider.hpp
        Final_Project/TurretButton.cpp Final_Project/TurretButton.hpp
        Final_Project/ViewController.h
        Final_Project/WoodBullet.cpp Final_Project/WoodBullet.hpp
        Final_Project/WinScene.cpp Final_Project/WinScene.hpp
        Final_Project/SettingScene.cpp Final_Project/SettingScene.hpp Final_Project/BlueNormalEnemy.hpp Final_Project/BlueNormalEnemy.cpp Final_Project/L2Turret.cpp Final_Project/L2Turret.hpp Final_Project/ShootEffect.cpp Final_Project/ShootEffect.hpp Final_Project/GoldenBullet.cpp Final_Project/GoldenBullet.hpp Final_Project/DiceEnemy.cpp Final_Project/DiceEnemy.hpp Final_Project/L3Turret.cpp Final_Project/L3Turret.hpp Final_Project/DoubleBullet.cpp Final_Project/DoubleBullet.hpp Final_Project/Shovel.cpp Final_Project/Shovel.hpp Final_Project/BossEnemy.cpp Final_Project/BossEnemy.hpp Final_Project/PoopEnemy.cpp Final_Project/PoopEnemy.hpp Final_Project/SmallRocket.cpp Final_Project/SmallRocket.hpp Final_Project/DevilEnemy.cpp Final_Project/DevilEnemy.hpp Final_Project/MoveTool.cpp Final_Project/MoveTool.hpp Final_Project/MoveTurret.cpp Final_Project/MoveTurret.hpp Final_Project/Tool.cpp Final_Project/Tool.hpp Final_Project/ToolButton.cpp Final_Project/ToolButton.hpp Final_Project/L0Turret.cpp Final_Project/L0Turret.hpp Final_Project/L4Turret.cpp Final_Project/L4Turret.hpp Final_Project/L5Turret.cpp Final_Project/L5Turret.hpp Final_Project/DiceEnemy_2.cpp Final_Project/DiceEnemy_2.hpp Final_Project/wolfknight.cpp Final_Project/wolfknight.hpp Final_Project/ScoreboardScene.hpp Final_Project/ScoreboardScene.cpp Final_Project/LaserTurret.hpp Final_Project/LaserTurret.cpp Final_Project/LaserBullet.hpp Final_Project/LaserBullet.cpp Final_Project/MissileTurret.cpp Final_Project/MissileBullet.cpp Final_Project/MachineGunTurret.hpp Final_Project/MachineGunTurret.cpp Final_Project/FireBullet.cpp Final_Project/input_field.hpp Final_Project/input_field.cpp)

target_include_directories(TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
target_compile_options(TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
target_link_libraries(TowerDefense ${ALLEGRO5_LIBRARIES})
target_link_directories(TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})