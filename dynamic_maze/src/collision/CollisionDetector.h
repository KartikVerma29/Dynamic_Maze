#pragma once
#include "../events/EventManager.h"
#include "../player/Player.h"
#include "../enemy/IEnemy.h"
#include <memory>
#include <vector>

class CollisionDetector
{
private:
   EventManager& eventManager;
   // std::vector<IEnemy*> enemies;
   Player& player;
   const std::vector<std::unique_ptr<IEnemy>>& enemies;
   
   public:
   CollisionDetector(EventManager& eventManager, Player& player,const std::vector<std::unique_ptr<IEnemy>>& enemies);
   void checkEnemyMovement(IEnemy* e);
   void checkCollisions();
};
