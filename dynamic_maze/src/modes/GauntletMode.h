#pragma once
#include "IGameMode.h"
#include "../enemy/IEnemy.h"
#include "../player/Lives.h"
#include "../collision/CollisionDetector.h"
#include "../enemy/pathfinding/PatrolPathfinder.h"
#include "../enemy/pathfinding/AStarPathfinder.h"
#include "../events/events/EnemyDefeatedEvent.h"
#include "../scoring/GauntletScoreCalculator.h"
#include "events/IEventListener.h"
#include "events/events/PlayerHitEvent.h"
#include "events/events/PlayerMovedEvent.h"

class GauntletMode:public IGameMode, public IEventListener<PlayerHitEvent>
{
private:
   std::vector<std::unique_ptr<IEnemy>> enemies;
   std::vector<float> respawnTimers;
   int maxEnemies=5;
   
   Lives lives{3,3};
   float survivalTime=0;
   GauntletScoreCalculator& gauntletScorer;   

   std::unique_ptr<CollisionDetector> collisionDetector;
   // PatrolPathfinder patrolPathfinder;
   AStarPathfinder astarPathfinder;

   float invincibilityTime = 0.0f;

   void spawnEnemy(int slotIndex);
   // void rebuildCollisionDetector();

public:

   GauntletMode(UIManager& uiManager ,IMazeMutator& mutator, GauntletScoreCalculator& scorer, EventManager& eventManager, IMazeGenerator& generator, int startLevel=1):
      IGameMode(mutator, scorer, eventManager, generator, uiManager), gauntletScorer(scorer) {}

   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onEvent(const PlayerHitEvent&) override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   bool isFinished() const override{ return finished;}
};
