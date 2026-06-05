#pragma once
#include "../events/EventManager.h"
#include "../player/Lives.h"

class UIManager{
private:
   EventManager& eventManager;
   int selectedIndex=0;
   float volume = 50.0f;
   int difficulty = 0;
public:
   UIManager(EventManager& eventManager);

   void drawHUD(int score);
   void drawLives(const Lives& lives);
   void drawTime(float deltaTime);
   void drawMainMenu();
   void drawSettings();
   void drawGameOver(int score);
   void drawLevelComplete(int score);
};
