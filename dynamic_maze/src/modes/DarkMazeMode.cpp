#include "DarkMazeMode.h"

void DarkMazeMode::init(){
   int mazeSize = calcMazeSize(currentLevel);
   maze = std::make_unique<Maze>(mazeSize, mazeSize);
   generator.generate(*maze);
   lightRadius = 5.0 * (mazeSize/50.0);

   player = std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
   exitPos.setX(mazeSize-1);
   exitPos.setY(mazeSize-1);

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
   eventManager.subscribe<PlayerHitEvent>(*player);
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
    eventManager.unsubcribe<PlayerHitEvent>(*player);
    eventManager.unsubcribe<WallStateChangedEvent>(*player);
}

void DarkMazeMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();
   renderer.drawMaze(*maze);
   renderer.drawPlayer(*player);
   Position pos = player->getPosition();
   renderer.drawFog(pos, lightRadius);
   renderer.endFrame();
}

void DarkMazeMode::onEnter(){ init();}

void DarkMazeMode::onExit(){ cleanup();}

void DarkMazeMode::update(float deltaTime){
   elapsedTime+=deltaTime;
   stepCounter++;
   totalsteps++;

   if(isDynamic){
      if(stepCounter==stepThreshold){
        stepCounter=0;
        mutator.mutate(*maze, player->getPosition(), exitPos);
        stepThreshold = 15+(rand()%10-5);
    }
   }

   if(player->getPosition()==exitPos){
      finished=true;

      int visited=0;
      for(int i=0; i<maze->getRows(); i++){
         for(int j=0; j<maze->getCols(); j++){
            if(maze->getCell(i,j)->isVisited()) visited++;
         }
      }

      mapCoverage = (float) visited/(maze->getCols()*maze->getRows());

      ScoreContext ctx;
      ctx.timeTaken=elapsedTime;
      ctx.pathLength=totalsteps;
      ctx.optimalPathLenght=optimalPath.size();
      ctx.mapCoverage = mapCoverage;
      scorer.calculate(ctx);
   }
}
