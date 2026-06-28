#include "DarkMazeMode.h"
#include "../events/events/PlayerMovedEvent.h"
#include <cstdlib>

// state = 2
void DarkMazeMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize, mazeSize);
   generator.generate(*maze);
   lightRadius = 7.0 * (mazeSize/50.0);

   player = std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
   player->setMaze(maze.get());
   exitPos.setX(mazeSize-1);
   exitPos.setY(mazeSize-1);

   finished=false;
   elapsedTime=0.0f;
   scorer.resetScore();
   levelCompleted=false;
   totalsteps=0;
   stepCounter=0;
   mapCoverage=0;

   optimalPath = checker.getPath(*maze, Position(0,0), exitPos);

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

   eventManager.subscribe<PlayerMovedEvent>(*player);
   // eventManager.subscribe<PlayerHitEvent>(*player);
   eventManager.subscribe<WallStateChangedEvent>(*player);
}

void DarkMazeMode::cleanup(){
    for(int r=0; r<maze->getRows(); r++){
        for(int c=0; c<maze->getCols(); c++){
            Cell* cell = maze->getCell(r,c);
            if(cell){
                eventManager.unsubcribe<WallAddedEvent>(*cell);
                eventManager.unsubcribe<WallRemovedEvent>(*cell);
                eventManager.unsubcribe<WallStateChangedEvent>(*cell);
            }
            
        }
    }

    eventManager.unsubcribe<PlayerMovedEvent>(*player);
   //  eventManager.unsubcribe<PlayerHitEvent>(*player);
    eventManager.unsubcribe<WallStateChangedEvent>(*player);
}

void DarkMazeMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();
   renderer.drawMaze(*maze);
   renderer.drawPlayer(*player);
   
   Position pos = player->getPosition();
   renderer.drawFog(pos, lightRadius);

   uiManager.drawHUD(scorer.getScore());

   if(levelCompleted){
      uiManager.drawLevelComplete(scorer.getScore());
   }

   renderer.endFrame();
}

void DarkMazeMode::onEnter(){ init();}

void DarkMazeMode::onExit(){ cleanup();}

void DarkMazeMode::update(float deltaTime){
   if(levelCompleted) return;
   
   player->updateVisual(deltaTime);
   elapsedTime+=deltaTime;

   if(player->getPosition()==exitPos && player->isVisualComplete()){
      levelCompleted=true;
      currentLevel++;
      float visited=0;
      for(int i=0; i<maze->getRows(); i++){
         for(int j=0; j<maze->getCols(); j++){
            if(maze->getCell(i,j)->isVisited()) visited++;
         }
      }

      mapCoverage = (visited)/((float)maze->getCols()*maze->getRows());

      ScoreContext ctx;
      ctx.timeTaken=elapsedTime;
      ctx.pathLength=totalsteps;
      ctx.optimalPathLenght=optimalPath.size();
      ctx.mapCoverage = mapCoverage;
      scorer.calculate(ctx);
   }
}

void DarkMazeMode::onEvent(const PlayerMovedEvent& event){
   stepCounter++;
   totalsteps++;
   if(isDynamic){
      if(stepCounter>=stepThreshold){
         stepCounter=0;
         mutator.mutate(*maze, player->getPosition(), exitPos);
         stepThreshold = 10+(rand()%10-5);
      }
   }
}