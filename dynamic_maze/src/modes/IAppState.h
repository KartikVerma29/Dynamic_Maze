#pragma once

class IRenderer;

enum class AppStateType {SETTINGS, MAINMENU, DARK, DYNAMIC, GAUNTLET};


class IAppState{
public:
   virtual void onEnter() = 0;
   virtual void onExit() = 0;
   virtual void update(float deltaTime) = 0;
   virtual void render(IRenderer& renderer) = 0;
   virtual bool isFinished() const = 0;
   virtual AppStateType getType()const = 0;

   virtual ~IAppState() = default;
};
