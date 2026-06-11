#pragma once
#include "../events/IEventListener.h"
#include "../events/events/PlayerMovedEvent.h"
#include "../events/EventManager.h"
#include "../player/Player.h"
#include "../enemy/IEnemy.h"

class CollisionDetector:public IEventListener<PlayerMovedEvent>
{
private:
   EventManager& eventManager;
   std::vector<IEnemy*> enemies;
   Player& player;

public:
   CollisionDetector(EventManager& eventManager, Player& player,std::vector<IEnemy*>& enemies);
   void checkCollisions();
   void onEvent(const PlayerMovedEvent &event) override;
};
