#pragma once
#include "../modes/IAppState.h"
#include "../events/EventManager.h"
#include "../renderer/IRenderer.h"
#include <map>

class ModeManager{
private:
   IAppState* currentState;
   std::map<AppStateType, IAppState*> states;
   EventManager& eventManager;
public:
   ModeManager(EventManager& eventManager): eventManager(eventManager){}

   void registerState(AppStateType stateType, IAppState* appState);
   void transitionTo(AppStateType stateType);
   void update(float deltaTime);
   void render(IRenderer& renderer);
   IAppState* getCurrentState();
};
