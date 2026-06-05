#pragma once
#include "ModeManager.h"
#include "../player/InputHandler.h"
class GameLoop{
private:
   ModeManager modeManager;
   IRenderer& renderer;
   EventManager& eventManager;
   InputHandler inputHandler;
   bool isRunning;
   float targetFPS;
   
   void processInput();
   void update(float deltaTime);
   void render();
   float calcDeltaTime();
public:

   GameLoop(IRenderer& renderer, EventManager& eventManager, float targetFPS=60):
      renderer(renderer), eventManager(eventManager), targetFPS(targetFPS) {
   
      }

   void run();
   void stop();

};
