#include "GauntletMode.h"

void GauntletMode::spawnEnemy(int slotIndex){

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

   if(chasers<currentLevel)
      enemies[slotIndex] = std::make_unique<Chaser>(astarPathfinder, Position(r,c), Direction(DirectionType::UP),*player); 
   else if(rand()%2==0){
      if(patrols<maxEnemies-2){
         std::vector<Position> route{Position(r,c)};
         for(int i=0; i<3; i++)
            route.push_back(Position(rand()%maze->getRows(), rand()%maze->getCols()));
         enemies[slotIndex] = std::make_unique<Patrol>(route, astarPathfinder, Position(r,c), Direction(DirectionType::UP));
      }
   }else if(blockers<maxEnemies-2){
      enemies[slotIndex]=std::make_unique<Blocker>(Position(r,c), Direction(DirectionType::DOWN));
   }
   if(enemies[slotIndex]){
      eventManager.subscribe<WallStateChangedEvent>(*enemies[slotIndex]);
   } 
}



void GauntletMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize,mazeSize);
   generator.generate(*maze);
   postProcessor.process(*maze);

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
   maxEnemies = BASE_ENEMIES*currentLevel;
   enemies.clear();
   respawnTimers.clear();
   for(int i=0; i<maxEnemies; i++){
      enemies.push_back(nullptr);
      respawnTimers.push_back(0.0f);
   }

   collisionDetector = std::make_unique<CollisionDetector>(eventManager,*player, enemies);

   eventManager.subscribe<EnemyDefeatedEvent>(gauntletScorer);
   eventManager.subscribe<PlayerMovedEvent>(*player);      
   eventManager.subscribe<PlayerHitEvent>(*player);
   eventManager.subscribe<WallStateChangedEvent>(*player);
   eventManager.subscribe<PlayerHitEvent>(*this);
}


void GauntletMode::cleanup(){

   eventManager.unsubscribe<EnemyDefeatedEvent>(gauntletScorer);
   eventManager.unsubscribe<PlayerMovedEvent>(*player);      
   eventManager.unsubscribe<PlayerHitEvent>(*player);
   eventManager.unsubscribe<WallStateChangedEvent>(*player);
   eventManager.unsubscribe<PlayerHitEvent>(*this);


   for(auto& e:enemies){
      if(e) eventManager.unsubscribe<WallStateChangedEvent>(*e);
   }
   enemies.clear();
   respawnTimers.clear();
}

void GauntletMode::onEnter(){ init(); }
void GauntletMode::onExit() { cleanup(); }

void GauntletMode::onEvent(const PlayerHitEvent&){
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
   player->updateVisual(deltaTime);

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
      e->updateVisual(deltaTime);
   }
   
   if(collisionDetector) collisionDetector->checkCollisions();
   
   for(int i=0; i<maxEnemies; i++){
      if(enemies[i] && enemies[i]->getIsDefeated()){
         eventManager.unsubscribe<WallStateChangedEvent>(*enemies[i]);
         enemies[i].reset();
         respawnTimers[i]=3.0f;
      }
   }

   if(lives.isGameOver()){
      eventManager.unsubscribe<PlayerMovedEvent>(*player);
      ScoreContext ctx;
      ctx.timeTaken = elapsedTime;
      gauntletScorer.calculate(ctx);
   }
   if(survivalTime<=elapsedTime){
      eventManager.unsubscribe<PlayerMovedEvent>(*player);
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

   renderer.setMazeDimensions(maze->getRows(), maze->getCols());

   renderer.beginWorld();
   renderer.drawMaze(*maze);
   renderer.drawPlayer(*player);

   for(auto& e:enemies){
      if(e) renderer.drawEnemy(*e);
   }

   renderer.endWorld();

   uiManager.drawTime(elapsedTime);
   uiManager.drawLives(lives);   
   uiManager.drawHUD(scorer.getScore());

   if(lives.isGameOver()) {
      uiManager.drawGameOver(scorer.getScore());
   }
   if(levelCompleted){
      uiManager.drawLevelComplete(scorer.getScore());
   }
   renderer.endFrame();
}
