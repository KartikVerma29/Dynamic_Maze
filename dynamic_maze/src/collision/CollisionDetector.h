#pragma once
#include "../events/IEventListener.h"
#include "../events/events/PlayerMovedEvent.h"
#include "../events/EventManager.h"
#include "../player/Player.h"
#include "../enemy/IEnemy.h"
#include <memory>
#include <vector>

class CollisionDetector:public IEventListener<PlayerMovedEvent>
{
private:
   EventManager& eventManager;
   // std::vector<IEnemy*> enemies;
   const std::vector<std::unique_ptr<IEnemy>>& enemies;
   Player& player;
   
   void checkCollisions();
public:
   CollisionDetector(EventManager& eventManager, Player& player,const std::vector<std::unique_ptr<IEnemy>>& enemies);
   void onEvent(const PlayerMovedEvent &event) override;
   void checkEnemyMovement(IEnemy* e);
};
