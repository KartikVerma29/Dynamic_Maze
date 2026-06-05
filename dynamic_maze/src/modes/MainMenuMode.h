#pragma once
#include "IAppState.h"
#include "../ui/UIManager.h"

class MainMenuMode: public IAppState{
private:
   AppStateType selectedMode;
   UIManager& uiManager;
public:
   MainMenuMode(UIManager& uiManager): uiManager(uiManager){}
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer &renderer) override;
   bool isFinished() const override;
   AppStateType getType() const override;

};
