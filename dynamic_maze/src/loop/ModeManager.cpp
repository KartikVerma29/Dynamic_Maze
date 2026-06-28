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
   int index = uiManager.getSelectedIndex();
   if(index!=-1){
      uiManager.resetSelectedIndex();
      switch (index) {
         case 0: transitionTo(AppStateType::MAINMENU); break;
         case 1: transitionTo(AppStateType::DYNAMIC); break;
         case 2: transitionTo(AppStateType::DARK); break;
         case 3: transitionTo(AppStateType::GAUNTLET); break;
         case 4: transitionTo(AppStateType::SETTINGS); break;
         case 10:
         case 20: 
            currentState->onExit();
            currentState->onEnter();
            break;
      }
      return;
   }
   
   if( currentState->isFinished()){
      AppStateType nextState = currentState->getNextState();
      transitionTo(nextState);
   }

}

void ModeManager::render(IRenderer& renderer){
   if(currentState==nullptr) return;
   currentState->render(renderer);
}

