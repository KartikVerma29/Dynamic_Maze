#include "SettingsMode.h"

// state = 4
void SettingsMode::onEnter(){
   finished=false;
   uiManager.resetSelectedIndex();
}

void SettingsMode::onExit(){
   uiManager.resetSelectedIndex();
}

void SettingsMode::render(IRenderer& renderer){
   renderer.beginFrame();
   renderer.clearScreen();
   uiManager.drawSettings();
   renderer.endFrame();
}

void SettingsMode::update(float deltaTime){
   int index = uiManager.getSelectedIndex();
   if(index == 0) finished=true;
}
