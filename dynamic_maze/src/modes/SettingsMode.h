#pragma once
#include "IAppState.h"
#include "../ui/UIManager.h"

class SettingsMode:public IAppState
{
private:
   UIManager& uiManager;
public:
   SettingsMode(UIManager& uiManager): uiManager(uiManager){}

   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer &renderer) override;
   bool isFinished() const override;
};
