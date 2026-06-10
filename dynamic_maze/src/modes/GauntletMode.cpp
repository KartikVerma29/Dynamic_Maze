#include "GauntletMode.h"
#include "../enemy/Chaser.h"
#include "../enemy/Blocker.h"
#include "../enemy/Patrol.h"
#include "events/events/EnemyDefeatedEvent.h"
#include "events/events/PlayerHitEvent.h"
#include "events/events/PlayerMovedEvent.h"
#include "events/events/WallAddedEvent.h"
#include "events/events/WallRemovedEvent.h"
#include "events/events/WallStateChangedEvent.h"
#include "renderer/IRenderer.h"
#include "scoring/IScoreCalculator.h"
#include <memory>

// state = 3

void GauntletMode::rebuildCollisionDetector(){
   std::vector<IEnemy*> ptrs;
   for(auto& e:enemies) ptrs.push_back(e.get());
   collisionDetector = std::make_unique<CollisionDetector>(eventManager, *player,ptrs);
}

void GauntletMode::spawnEnemy(){
   if((int)enemies.size()>=maxEnemies) return;

   int chasers=0, patrols=0, blockers=0;

   for(auto& e:enemies){
      switch (e->getType()){
         case EnemyType::CHASER:chasers++; break;
         case EnemyType::BLOCKER:blockers++; break;
         case EnemyType::PATROL:patrols++; break;
      }
   }
   int r = rand()%maze->getRows();
   int c = rand()%maze->getCols();

   if(chasers<1)
      enemies.push_back(std::make_unique<Chaser>(astarPathfinder, Position(r,c), Direction(DirectionType::UP)));
   else if(patrols<3)
      enemies.push_back(std::make_unique<Patrol>(astarPathfinder, Position(r,c), Direction(DirectionType::UP)));
   else if(blockers<3)
      enemies.push_back(std::make_unique<Blocker>(Position(r,c), Direction(DirectionType::UP)));
   
   rebuildCollisionDetector();
}

void GauntletMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize,mazeSize);
   generator.generate(*maze);

   player=std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
   lives = Lives(3,3);

   rebuildCollisionDetector();
   for(int r=0; r<maze->getRows(); r++){
        for(int c=0; c<maze->getCols(); c++){
            Cell* cell = maze->getCell(r,c);
            if(cell){
                eventManager.subscribe<WallAddedEvent>(*cell);
                eventManager.subscribe<WallRemovedEvent>(*cell);
                eventManager.subscribe<WallStateChangedEvent>(*cell);
            }
        }
    }
   eventManager.subscribe<EnemyDefeatedEvent>(gauntletScorer);
   eventManager.subscribe<PlayerMovedEvent>(*player);      
   eventManager.subscribe<PlayerHitEvent>(*player);
   eventManager.subscribe<WallStateChangedEvent>(*player);
}


void GauntletMode::cleanup(){
   for(int i=0;i<maze->getRows(); i++){
      for(int j=0; j<maze->getCols(); j++){
         Cell* cell = maze->getCell(i, j);
         if(cell){
            eventManager.unsubcribe<WallAddedEvent>(*cell);
            eventManager.unsubcribe<WallRemovedEvent>(*cell);
            eventManager.unsubcribe<WallStateChangedEvent>(*cell);
         }
      }
   }
   
   eventManager.unsubcribe<PlayerMovedEvent>(*player);      
   eventManager.unsubcribe<PlayerHitEvent>(*player);
   eventManager.unsubcribe<WallStateChangedEvent>(*player);
}

void GauntletMode::onEnter(){ init(); }
void GauntletMode::onExit() { cleanup(); }

void GauntletMode::update(float deltaTime){
   elapsedTime+=deltaTime;
   survivalTime+=deltaTime;
   spawnTimer+=deltaTime;

   if(spawnTimer>=spawnInterval){
      spawnTimer=0.0f;
      spawnEnemy();
      spawnInterval = std::max(3.0f, spawnInterval-0.5f);
   }

   if(lives.isGameOver()){
      finished=true;
      ScoreContext ctx;
      ctx.timeTaken = survivalTime;
      gauntletScorer.calculate(ctx);
   }

}

void GauntletMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();
   renderer.drawMaze(*maze);
   renderer.drawPlayer(*player);

   for(auto& e:enemies){
      renderer.drawEnemy(*e);
   }
   renderer.endFrame();
}
