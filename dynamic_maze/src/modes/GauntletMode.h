#pragma once
#include "IGameMode.h"
#include "../enemy/IEnemy.h"
#include "../player/Lives.h"
#include "../collision/CollisionDetector.h"
#include "../enemy/pathfinding/IPathfinder.h"
#include "../enemy/pathfinding/AStarPathfinder.h"
#include "../enemy/pathfinding/heuristic/IHeuristic.h"
#include "../enemy/pathfinding/heuristic/ManhattanHeuristic.h"
#include "../events/events/EnemyDefeatedEvent.h"
#include "../scoring/GauntletScoreCalculator.h"
#include "../events/IEventListener.h"
#include "../events/events/PlayerHitEvent.h"


class GauntletMode:public IGameMode, public IEventListener<PlayerHitEvent>
{
private:
   std::vector<std::unique_ptr<IEnemy>> enemies;
   std::vector<float> respawnTimers;
   static constexpr int BASE_ENEMIES =5;
   int maxEnemies=BASE_ENEMIES;

   Lives lives{3,3};
   float survivalTime=0;
   GauntletScoreCalculator& gauntletScorer;   

   std::unique_ptr<CollisionDetector> collisionDetector;
   IPathfinder& astarPathfinder;

   float invincibilityTime = 0.0f;

   void spawnEnemy(int slotIndex);
   // void rebuildCollisionDetector();

public:

   GauntletMode(UIManager& uiManager ,IMazeMutator& mutator, 
         GauntletScoreCalculator& scorer, EventManager& eventManager, 
         IMazeGenerator& generator, IPathfinder& finder, IMazePostProcessor& postProcessor,int startLevel=1):
      IGameMode(mutator, scorer, eventManager, generator, uiManager, postProcessor), 
      gauntletScorer(scorer), astarPathfinder(finder) {}

   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onEvent(const PlayerHitEvent&) override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   bool isFinished() const override{ return finished;}
};
