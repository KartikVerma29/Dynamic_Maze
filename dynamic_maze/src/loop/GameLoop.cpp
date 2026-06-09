#include "GameLoop.h"
#include <raylib.h>


void GameLoop::processInput(){
   inputHandler->update();
}

void GameLoop::update(float deltaTime){
   modeManager->update(deltaTime);
}

void GameLoop::render(){
   modeManager->render(renderer);
}

float GameLoop::calcDeltaTime(){ return GetFrameTime(); }

void GameLoop::run(){
   while(!WindowShouldClose() && isRunning){
      float deltaTime = calcDeltaTime();
      processInput();
      update(deltaTime);
      render();

      IAppState* current = modeManager->getCurrentState();
      if(current && current->isFinished()){
         modeManager->transitionTo(current->getNextState());
      }
   }
}
