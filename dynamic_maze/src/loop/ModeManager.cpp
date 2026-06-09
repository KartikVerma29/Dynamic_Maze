#include "ModeManager.h"

void ModeManager::registerState(AppStateType stateType, IAppState* appState){
   states[stateType] = appState;
   if(currentState == nullptr) currentState = appState;
}

void ModeManager::transitionTo(AppStateType stateType){
   if(states.count(stateType) == 0) return;
   if(currentState!=nullptr) currentState->onExit();
   currentState = states[stateType];
   currentState->onEnter();
}

void ModeManager::update(float deltaTime){
   if(currentState==nullptr) return ;

   currentState->update(deltaTime);
   if( currentState->isFinished()){
      AppStateType nextState = currentState->getNextState();
      transitionTo(nextState);
   }
}

void ModeManager::render(IRenderer& renderer){
   if(currentState==nullptr) return;
   currentState->render(renderer);
}

