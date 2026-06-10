#include "MainMenuMode.h"

// state = 0

void MainMenuMode::onEnter(){
   finished=false;
   uiManager.resetSelectedIndex();
}

void MainMenuMode::onExit(){
   uiManager.resetSelectedIndex();
}

void MainMenuMode::update(float deltaTime){
   int index = uiManager.getSelectedIndex();
   switch(index){
      case 1: nextState=AppStateType::DYNAMIC; finished=true; break;
      case 2: nextState=AppStateType::DARK; finished=true; break;
      case 3: nextState=AppStateType::GAUNTLET; finished=true; break;
      case 4: nextState=AppStateType::SETTINGS; finished=true; break;
   }
}

void MainMenuMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();
   uiManager.drawMainMenu();
   renderer.endFrame();
}
