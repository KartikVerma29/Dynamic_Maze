#include "DarkMazeMode.h"
#include "../events/events/PlayerMovedEvent.h"
#include <cstdlib>

// state = 2
void DarkMazeMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize, mazeSize);
   generator.generate(*maze);
   postProcessor.process(*maze);

   maze->getCell(0, 0)->setExplored(true);

   lightRadius = 1.5f + (mazeSize / 50.0f) * 2.0f;
   player = std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
   player->setMaze(maze.get());
   exitPos.setX(mazeSize-1);
   exitPos.setY(mazeSize-1);
   std::printf("Light Radius: %f", lightRadius);
   finished=false;
   elapsedTime=0.0f;
   scorer.resetScore();
   levelCompleted=false;
   totalsteps=0;
   stepCounter=0;
   mapCoverage=0;

   optimalPath = checker.getPath(*maze, Position(0,0), exitPos);

   eventManager.subscribe<PlayerMovedEvent>(*player);
   // eventManager.subscribe<PlayerHitEvent>(*player);
   eventManager.subscribe<WallStateChangedEvent>(*player);
   eventManager.subscribe<PlayerMovedEvent>(*this);
}

void DarkMazeMode::cleanup(){
    
    eventManager.unsubscribe<PlayerMovedEvent>(*this);
    eventManager.unsubscribe<PlayerMovedEvent>(*player);
   //  eventManager.unsubscribe<PlayerHitEvent>(*player);
    eventManager.unsubscribe<WallStateChangedEvent>(*player);
}

void DarkMazeMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();

   renderer.setMazeDimensions(maze->getRows(), maze->getCols());

   FloatPos renderPos = player->getRenderPosition();
   Position smoothPos(renderPos.x, renderPos.y);
   renderer.updateFog(smoothPos, lightRadius);

   renderer.beginWorld();
   renderer.drawMaze(*maze);
   renderer.drawExit(exitPos);
   renderer.drawPlayer(*player);
   renderer.drawFog();
   
   renderer.endWorld();

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
      eventManager.unsubscribe<PlayerMovedEvent>(*player);
      float visited=0;
      for(int i=0; i<maze->getRows(); i++){
         for(int j=0; j<maze->getCols(); j++){
            if(maze->getCell(i,j)->isExplored()) visited++;
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

void DarkMazeMode::onEvent(const PlayerMovedEvent&){
   if(levelCompleted) return;
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