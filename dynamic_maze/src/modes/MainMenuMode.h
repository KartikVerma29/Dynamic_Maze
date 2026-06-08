#pragma once
#include "IAppState.h"
#include "../ui/UIManager.h"

class MainMenuMode: public IAppState{
private:
   UIManager& uiManager;
   AppStateType nextState;
   bool finished=false;
public:
   MainMenuMode(UIManager& uiManager): uiManager(uiManager){}
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer &renderer) override;
   bool isFinished() const override { return finished;}
   AppStateType getType() const override{ return AppStateType::MAINMENU ;}
   AppStateType getNextState() const override{ return nextState;}

};
