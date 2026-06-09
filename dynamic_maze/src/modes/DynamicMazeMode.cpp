#include "DynamicMazeMode.h"

void DynamicMazeMode::init(){
    int mazeSize = calcMazeSize(currentLevel);
    maze = std::make_unique<Maze>(mazeSize,mazeSize);
    generator.generate(*maze);
    finished=false;
    player = std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
    exitPos.setX(mazeSize-1);
    exitPos.setY(mazeSize-1);
    
    player->setMaze(maze.get());
    optimalPath = checker.getPath(*maze, Position(0,0),exitPos);

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
    eventManager.subscribe<PlayerMovedEvent>(*this);
}

void DynamicMazeMode::cleanup(){
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

void DynamicMazeMode::update(float deltaTime){
    elapsedTime+=deltaTime;
    
    if(player->getPosition() == exitPos){
        finished=true;

        ScoreContext ctx;
        ctx.timeTaken = elapsedTime;
        ctx.pathLength = totalsteps;
        ctx.optimalPathLenght = optimalPath.size();
        scorer.calculate(ctx);
    }
}

void DynamicMazeMode::render(IRenderer& renderer){
    renderer.beginFrame();
    renderer.clearScreen();
    renderer.drawMaze(*maze);
    renderer.drawPlayer(*player);

    if( finished)
        uiManager.drawLevelComplete(scorer.getScore());
    else uiManager.drawHUD(scorer.getScore());

    renderer.endFrame();
}

void DynamicMazeMode::onEnter(){ init(); }

void DynamicMazeMode::onExit(){ cleanup(); }

void DynamicMazeMode::onEvent(const PlayerMovedEvent& event){
    stepCounter++;
    totalsteps++;
    if(stepCounter>=stepThreshold){
        stepCounter=0;
        mutator.mutate(*maze, player->getPosition(), exitPos);
        stepThreshold = 15+(rand()%10-5);
    }
}
