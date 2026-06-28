#include "GameLoop.h"
#include <raylib.h>

#if defined(PLATFORM_WEB)
   #include <emscripten/emscripten.h>
#endif

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

void GameLoop::step(){
   float deltaTime = calcDeltaTime();
   processInput();
   update(deltaTime);
   render();

   IAppState* current = modeManager->getCurrentState();
   if(current && current->isFinished()){
      modeManager->transitionTo(current->getNextState());
   }

}

#if defined (PLATFORM_WEB)
void UpdateDrawFrame(void* arg){
   GameLoop* loop = static_cast<GameLoop*>(arg);
   loop->step();
}
#endif

void GameLoop::run(){

   #if defined(PLATFORM_WEB)
      emscripten_set_main_loop_arg(UpdateDrawFrame, this, 0, 1);
   #else
      while(!WindowShouldClose() && isRunning){
         step();
      }
   #endif
}
