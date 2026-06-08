#pragma once
#include "IAppState.h"
#include "../ui/UIManager.h"

class SettingsMode:public IAppState
{
private:
   UIManager& uiManager;
   bool finished=false;
public:
   SettingsMode(UIManager& uiManager): uiManager(uiManager){}

   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer &renderer) override;
   AppStateType getNextState() const override { return AppStateType::MAINMENU;}
   AppStateType getType() const override { return AppStateType::SETTINGS;}
   bool isFinished() const override{ return finished;}
};


