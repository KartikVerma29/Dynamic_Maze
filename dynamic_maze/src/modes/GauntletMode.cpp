#include "GauntletMode.h"
#include "../enemy/Chaser.h"
#include "../enemy/Blocker.h"
#include "../enemy/Patrol.h"
#include "enemy/IEnemy.h"
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
      enemies.push_back(std::make_unique<Chaser>(astarPathfinder, Position(r,c), Direction(DirectionType::UP),*player));
   // else if(patrols<3)
      // enemies.push_back(std::make_unique<Patrol>(astarPathfinder, Position(r,c), Direction(DirectionType::UP)));
   else if(blockers<3)
      enemies.push_back(std::make_unique<Blocker>(Position(r,c), Direction(DirectionType::UP)));
   
      IEnemy* newEnemy = enemies.back().get();
      eventManager.subscribe<WallStateChangedEvent>(*newEnemy);
   rebuildCollisionDetector();
}

void GauntletMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize,mazeSize);
   generator.generate(*maze);
   
   finished=false;
   elapsedTime=0.0f;
   totalsteps=0;
   stepCounter=0;
   scorer.resetScore();
   levelCompleted=false;

   player=std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
   player->setMaze(maze.get());
   lives = Lives(3,3);

   spawnTimer = spawnInterval;

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
   eventManager.subscribe<PlayerHitEvent>(*this);
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
   eventManager.unsubcribe<PlayerHitEvent>(*this);
   for(auto& e:enemies){
      eventManager.unsubcribe<WallStateChangedEvent>(*e);
   }
   enemies.clear();
}

void GauntletMode::onEnter(){ init(); }
void GauntletMode::onExit() { cleanup(); }

void GauntletMode::onEvent(const PlayerHitEvent& event){
   if(invincibilityTime<=0.0f){
      lives.decrement();
      invincibilityTime=2.0f;
   }
}

void GauntletMode::update(float deltaTime){
   elapsedTime+=deltaTime;
   survivalTime+=deltaTime;
   spawnTimer+=deltaTime;

   if(invincibilityTime>0.0f){
      invincibilityTime-=deltaTime;
   }
   
   if(spawnTimer>=spawnInterval){
      spawnTimer=0.0f;
      spawnEnemy();
      spawnInterval = std::max(3.0f, spawnInterval-0.5f);
   }
   
   for(auto& e:enemies){
      e->update(*maze,deltaTime);
   }
   
   if(collisionDetector) collisionDetector->checkCollisions();
   uiManager.drawTime(deltaTime);
   uiManager.drawLives(lives);
   if(lives.isGameOver()){
      levelCompleted=true;
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

   uiManager.drawHUD(scorer.getScore());
   for(auto& e:enemies){
      renderer.drawEnemy(*e);
   }

   if(levelCompleted) uiManager.drawGameOver(scorer.getScore());

   renderer.endFrame();
}
