#pragma once
#include "ModeManager.h"
#include "../player/InputHandler.h"
#include "../ui/UIManager.h"
#include <memory>
class GameLoop{
private:

   std::unique_ptr<ModeManager> modeManager;
   IRenderer& renderer;
   EventManager& eventManager;
   std::unique_ptr<InputHandler> inputHandler;
   float targetFPS;
   bool isRunning;
   
   void processInput();
   void update(float deltaTime);
   void render();
   float calcDeltaTime();
public:

   GameLoop(IRenderer& renderer, EventManager& eventManager, UIManager& uiManager ,float targetFPS=60):
      renderer(renderer), eventManager(eventManager), targetFPS(targetFPS), isRunning(true) {
         modeManager = std::make_unique<ModeManager>(uiManager);
         inputHandler = std::make_unique<InputHandler>(eventManager);
      }

   void run();
   void stop(){ isRunning=false;}
   void step();
   ModeManager& getModeManager(){ return *modeManager;}

};
