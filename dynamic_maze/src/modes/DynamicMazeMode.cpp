#include "DynamicMazeMode.h"
#include "../events/events/PlayerMovedEvent.h"
#include <raylib.h>
// state = 1

void DynamicMazeMode::init(){
    int mazeSize = calcMazeSize(currentLevel);
    maze = std::make_unique<Maze>(mazeSize,std::ceil(1.2*mazeSize));
    generator.generate(*maze);
    postProcessor.process(*maze);

    eventManager.resetStats();

    finished=false;
    levelCompleted=false;
    elapsedTime=0.0f;
    totalsteps=0;
    stepCounter=0;
    scorer.resetScore();
    player = std::make_unique<Player>(Position(0,0), Direction(DirectionType::RIGHT));
    exitPos.setX(mazeSize-1);
    exitPos.setY(std::ceil(1.2*mazeSize)-1);
    player->setMaze(maze.get());
    optimalPath = checker.getPath(*maze, Position(0,0),exitPos);

    eventManager.subscribe<PlayerMovedEvent>(*player);
    // eventManager.subscribe<PlayerHitEvent>(*player);
    eventManager.subscribe<WallStateChangedEvent>(*player);
    eventManager.subscribe<PlayerMovedEvent>(*this);
    eventManager.printStats();
    mutator.clearPending();
}

void DynamicMazeMode::cleanup(){
    eventManager.resetStats();
    eventManager.unsubscribe<PlayerMovedEvent>(*player);
    // eventManager.unsubscribe<PlayerHitEvent>(*player);
    eventManager.unsubscribe<WallStateChangedEvent>(*player);

    eventManager.unsubscribe<PlayerMovedEvent>(*this);
    eventManager.printStats();
    mutator.clearPending();
}

void DynamicMazeMode::update(float deltaTime){
    if(levelCompleted) {
        return;
    }
    player->updateVisual(deltaTime);
    elapsedTime+=deltaTime;
    mutator.update(deltaTime, *maze, player->getPosition(), exitPos);
    
    if(player->getPosition() == exitPos && player->isVisualComplete()){
        levelCompleted=true;
        currentLevel++;
        eventManager.unsubscribe<PlayerMovedEvent>(*player);
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
    renderer.drawExit(exitPos);
    renderer.drawTelegraphs();
    renderer.drawPlayer(*player);
    renderer.endWorld();

    uiManager.drawHUD(scorer.getScore());
    if(levelCompleted){
        renderer.clearTelegraphs();
        uiManager.drawLevelComplete(scorer.getScore());
    } 

    renderer.endFrame();
}

void DynamicMazeMode::onEnter(){ init(); }

void DynamicMazeMode::onExit(){ cleanup(); }

void DynamicMazeMode::onEvent(const PlayerMovedEvent&){
    if(levelCompleted) return;
    stepCounter++;
    totalsteps++;
    if(stepCounter>=stepThreshold){
        stepCounter=0;
        mutator.mutate(*maze, player->getPosition(), exitPos);
        stepThreshold = 10+(rand()%10-5);
    }
}
