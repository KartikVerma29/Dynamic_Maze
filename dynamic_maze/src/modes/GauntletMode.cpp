#include "GauntletMode.h"
#include "../enemy/Chaser.h"
#include "../enemy/Blocker.h"
#include "../enemy/Patrol.h"
#include "collision/CollisionDetector.h"
#include "core/Position.h"
#include "enemy/IEnemy.h"
#include "events/events/EnemyDefeatedEvent.h"
#include "events/events/PlayerHitEvent.h"
#include "events/events/PlayerMovedEvent.h"
#include "events/events/WallAddedEvent.h"
#include "events/events/WallRemovedEvent.h"
#include "events/events/WallStateChangedEvent.h"
#include "renderer/IRenderer.h"
#include "scoring/IScoreCalculator.h"
#include <cstdlib>
#include <memory>

// state = 3

// void GauntletMode::rebuildCollisionDetector(){
//    std::vector<IEnemy*> ptrs;
//    for(auto& e:enemies) ptrs.push_back(e.get());
//    collisionDetector = std::make_unique<CollisionDetector>(eventManager, *player,ptrs);
// }

void GauntletMode::spawnEnemy(int slotIndex){
   // if((int)enemies.size()>=maxEnemies) return;

   int chasers=0, patrols=0, blockers=0;

   for(auto& e:enemies){
      if(!e) continue;
      switch (e->getType()){
         case EnemyType::CHASER:chasers++; break;
         case EnemyType::BLOCKER:blockers++; break;
         case EnemyType::PATROL:patrols++; break;
      }
   }
   int r = rand()%maze->getRows();
   int c = rand()%maze->getCols();

   int randomEnemy = rand()%2;
   if(chasers<currentLevel)
      enemies[slotIndex] = std::make_unique<Chaser>(astarPathfinder, Position(r,c), Direction(DirectionType::UP),*player); 

   switch(randomEnemy){
      case 0:{
         if(patrols<maxEnemies-2){
            std::vector<Position> route;
            route.push_back(Position(r,c));
            for(int i=0; i<3; i++){
               route.push_back(Position(rand()%maze->getRows(), rand()%maze->getCols()));
            }
            enemies[slotIndex] = std::make_unique<Patrol>(route ,Position(r,c), Direction(DirectionType::UP));
         }
      }
      case 1:{
         if(blockers<maxEnemies-2)
            enemies[slotIndex]= std::make_unique<Blocker>(Position(r,c), Direction(DirectionType::UP));
      }
   }

   if(enemies[slotIndex]){
      eventManager.subscribe<WallStateChangedEvent>(*enemies[slotIndex]);
   }
   // rebuildCollisionDetector();
}

void GauntletMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize,mazeSize);
   generator.generate(*maze);

   player=std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
   player->setMaze(maze.get());
   lives = Lives(3,3);
   
   finished=false;
   elapsedTime=0.0f;
   survivalTime=(float)(30.0f)*(1.0f+0.5*currentLevel);

   // totalsteps=0;
   // stepCounter=0;
   scorer.resetScore();
   levelCompleted=false;

   // spawnTimer = spawnInterval;
   maxEnemies = maxEnemies*(currentLevel);
   enemies.clear();
   respawnTimers.clear();
   for(int i=0; i<maxEnemies; i++){
      enemies.push_back(nullptr);
      respawnTimers.push_back(0.0f);
   }

   // rebuildCollisionDetector();

   collisionDetector = std::make_unique<CollisionDetector>(eventManager,*player, enemies);

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
      if(e) eventManager.unsubcribe<WallStateChangedEvent>(*e);
   }
   enemies.clear();
   respawnTimers.clear();
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
   if(lives.isGameOver() || levelCompleted) return;
      if(invincibilityTime>0.0f){
      invincibilityTime-=deltaTime;
   }
   
   elapsedTime+=deltaTime;

   for(int i=0; i<maxEnemies; i++){
      if(!enemies[i]){
         respawnTimers[i]-=deltaTime;
         if(respawnTimers[i]<=0.0f) spawnEnemy(i);
      }
   }

   if(collisionDetector) collisionDetector->checkCollisions();

   for(auto& e:enemies){
      if(!e) continue;
      e->update(*maze,deltaTime);
   }
   
   if(collisionDetector) collisionDetector->checkCollisions();
   
   for(int i=0; i<maxEnemies; i++){
      if(enemies[i] && enemies[i]->getIsDefeated()){
         eventManager.unsubcribe<WallStateChangedEvent>(*enemies[i]);
         enemies[i].reset();
         respawnTimers[i]=3.0f;
      }
   }

   if(lives.isGameOver()){
      ScoreContext ctx;
      ctx.timeTaken = elapsedTime;
      gauntletScorer.calculate(ctx);
   }
   if(survivalTime<=elapsedTime){
      levelCompleted=true;
      currentLevel++;
      ScoreContext ctx;
      ctx.timeTaken = elapsedTime;
      gauntletScorer.calculate(ctx);
   }

}

void GauntletMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();
   renderer.drawMaze(*maze);
   renderer.drawPlayer(*player);
   uiManager.drawTime(elapsedTime);
   uiManager.drawLives(lives);   
   uiManager.drawHUD(scorer.getScore());

   for(auto& e:enemies){
      if(e) renderer.drawEnemy(*e);
   }
   if(lives.isGameOver()) {
      uiManager.drawGameOver(scorer.getScore());
   }
   if(levelCompleted){
      uiManager.drawLevelComplete(scorer.getScore());
   }
   renderer.endFrame();
}
