#include "CollisionDetector.h"
#include "../events/events/PlayerHitEvent.h"
#include "../events/events/EnemyDefeatedEvent.h"
#include <memory>

const float COLLISION_THREHOLD=0.6f;

static float caluFloatDistance(FloatPos a, FloatPos b){
   float dx=a.x - b.x;
   float dy = a.y-b.y;
   return std::sqrt(dx*dx+dy*dy);
}

CollisionDetector::CollisionDetector(EventManager& eventManager, Player& player,const std::vector<std::unique_ptr<IEnemy>>&enemies) :
   eventManager(eventManager), player(player), enemies(enemies) {}

void CollisionDetector::checkCollisions(){
   if(enemies.empty()) return ;
   FloatPos pPos = player.getRenderPosition();
   for(const auto& e:enemies){

      if(!e || e->getIsDefeated()) continue;
      FloatPos ePos = e->getRenderPosition();
      if(caluFloatDistance(pPos, ePos) <COLLISION_THREHOLD){
         if(e->isDefeatable(player.getDirection())){
            e->setDefeated();
            eventManager.publish<EnemyDefeatedEvent>({e.get(),e->getPosition()});
         }else eventManager.publish<PlayerHitEvent>({player.getPosition(), e.get()});
      }
   }

}

void CollisionDetector::checkEnemyMovement(IEnemy* e){
   if(!e|| e->getIsDefeated()) return;

   FloatPos pPos = player.getRenderPosition();
   FloatPos ePos = e->getRenderPosition();

   if(caluFloatDistance(pPos, ePos)<COLLISION_THREHOLD){
      if(e->isDefeatable(player.getDirection())){
         e->setDefeated();
         eventManager.publish<EnemyDefeatedEvent>({e,e->getPosition()});
      }else eventManager.publish<PlayerHitEvent>({player.getPosition(),e});
   }
}
