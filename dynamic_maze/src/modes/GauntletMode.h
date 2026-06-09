#pragma once
#include "IGameMode.h"
#include "../enemy/IEnemy.h"
#include "../player/Lives.h"
#include "../collision/CollisionDetector.h"
#include "../enemy/pathfinding/PatrolPathfinder.h"
#include "../enemy/pathfinding/AStarPathfinder.h"
#include "../events/events/EnemyDefeatedEvent.h"
#include "../scoring/GauntletScoreCalculator.h"

class GauntletMode:public IGameMode
{
private:
   std::vector<std::unique_ptr<IEnemy>> enemies;
   Lives lives{3,3};
   float survivalTime=0;
   GauntletScoreCalculator& gauntletScorer;   

   float spawnTimer = 0.0f;
   float spawnInterval=10.0f;
   int maxEnemies=5;
   std::unique_ptr<CollisionDetector> collisionDetector;

   // PatrolPathfinder patrolPathfinder;
   AStarPathfinder astarPathfinder;

   void spawnEnemy();
   void rebuildCollisionDetector();

public:

   GauntletMode(UIManager uiManager ,IMazeMutator& mutator, GauntletScoreCalculator& scorer, EventManager& eventManager, IMazeGenerator& generator, int startLevel=1):
      IGameMode(mutator, scorer, eventManager, generator, uiManager), gauntletScorer(scorer) {}

   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   bool isFinished() const override{ return finished;}
};
