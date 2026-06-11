#include "CollisionDetector.h"
#include "../events/events/PlayerHitEvent.h"
#include "../events/events/EnemyDefeatedEvent.h"

const float COLLISION_THREHOLD=0.8f;

CollisionDetector::CollisionDetector(EventManager& eventManager, Player& player, std::vector<IEnemy*>&enemies) :
   eventManager(eventManager), player(player), enemies(enemies) {}

void CollisionDetector::checkCollisions(){
   if(enemies.empty()) return ;
   for(IEnemy* e:enemies){

      if(e->getIsDefeated()) continue;

      if(player.getPosition().distanceTo(e->getPosition())<COLLISION_THREHOLD){
         if(e->isDefeatable(player.getDirection())){
            eventManager.publish<EnemyDefeatedEvent>({e,e->getPosition()});
         }else eventManager.publish<PlayerHitEvent>({player.getPosition(), e});
      }
   }

}

void CollisionDetector::onEvent(const PlayerMovedEvent& event){
   checkCollisions();
}
