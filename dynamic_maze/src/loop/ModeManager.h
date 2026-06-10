#pragma once
#include "../modes/IAppState.h"
#include "../events/EventManager.h"
#include "../renderer/IRenderer.h"
#include "../ui/UIManager.h"
#include <map>

class ModeManager{
private:
   IAppState* currentState;
   std::map<AppStateType, IAppState*> states;
   EventManager& eventManager;
   UIManager& uiManager;
public:
   ModeManager(EventManager& eventManager, UIManager& uiManager): eventManager(eventManager), uiManager(uiManager){
      currentState=nullptr;
   }

   void registerState(AppStateType stateType, IAppState* appState);
   void transitionTo(AppStateType stateType);
   void update(float deltaTime);
   void render(IRenderer& renderer);
   IAppState* getCurrentState(){ return currentState; }
};
