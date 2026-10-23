#pragma once
#include "../player/Lives.h"
#include <string>

#include <raylib.h>

class UIManager{
private:
   int selectedIndex=0;
   float volume = 50.0f;
   int difficulty = 0;
public:
   UIManager();
   
   void drawHUD(int score);
   void drawLives(const Lives& lives);
   void drawTime(float deltaTime);
   void drawMainMenu();
   void drawSettings();
   void drawGameOver(int score);
   void drawLevelComplete(int score);

   int getSelectedIndex() const { return selectedIndex;}
   void resetSelectedIndex() { selectedIndex = -1;}

};
