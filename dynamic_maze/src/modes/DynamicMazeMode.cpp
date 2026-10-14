#include "DynamicMazeMode.h"
#include "../events/events/PlayerMovedEvent.h"
// state = 1

void DynamicMazeMode::init(){
    int mazeSize = calcMazeSize(currentLevel);
    maze = std::make_unique<Maze>(mazeSize,mazeSize);
    generator.generate(*maze);
    postProcessor.process(*maze);

    finished=false;
    levelCompleted=false;
    elapsedTime=0.0f;
    totalsteps=0;
    stepCounter=0;
    scorer.resetScore();
    player = std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
    exitPos.setX(mazeSize-1);
    exitPos.setY(mazeSize-1);
    
    player->setMaze(maze.get());
    optimalPath = checker.getPath(*maze, Position(0,0),exitPos);

    eventManager.subscribe<PlayerMovedEvent>(*player);
    // eventManager.subscribe<PlayerHitEvent>(*player);
    eventManager.subscribe<WallStateChangedEvent>(*player);
    eventManager.subscribe<PlayerMovedEvent>(*this);
}

void DynamicMazeMode::cleanup(){

    eventManager.unsubscribe<PlayerMovedEvent>(*player);
    // eventManager.unsubscribe<PlayerHitEvent>(*player);
    eventManager.unsubscribe<WallStateChangedEvent>(*player);

    eventManager.unsubscribe<PlayerMovedEvent>(*this);
}

void DynamicMazeMode::update(float deltaTime){
    if(levelCompleted) {
        return;
    }
    player->updateVisual(deltaTime);
    elapsedTime+=deltaTime;
    
    if(player->getPosition() == exitPos && player->isVisualComplete()){
        levelCompleted=true;
        currentLevel++;
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

    renderer.setMazeDimensions(maze->getRows(), maze->getCols());

    renderer.beginWorld();
    renderer.drawMaze(*maze);
    renderer.drawPlayer(*player);
    renderer.endWorld();

    uiManager.drawHUD(scorer.getScore());
    if(levelCompleted){
        uiManager.drawLevelComplete(scorer.getScore());
    } 

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
        stepThreshold = 10+(rand()%10-5);
    }
}
