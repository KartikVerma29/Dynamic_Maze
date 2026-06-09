#pragma once
#include "ModeManager.h"
#include "../player/InputHandler.h"
#include <memory>
class GameLoop{
private:

   std::unique_ptr<ModeManager> modeManager;
   IRenderer& renderer;
   EventManager& eventManager;
   std::unique_ptr<InputHandler> inputHandler;
   bool isRunning;
   float targetFPS;
   
   void processInput();
   void update(float deltaTime);
   void render();
   float calcDeltaTime();
public:

   GameLoop(IRenderer& renderer, EventManager& eventManager, float targetFPS=60):
      renderer(renderer), eventManager(eventManager), targetFPS(targetFPS), isRunning(true) {
         modeManager = std::make_unique<ModeManager>(eventManager);
         inputHandler = std::make_unique<InputHandler>(eventManager);
      }

   void run();
   void stop(){ isRunning=false;}

};
