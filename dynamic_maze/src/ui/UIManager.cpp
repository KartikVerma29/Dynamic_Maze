
#include <string>
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#include "UIManager.h"
#include <raylib.h>
#include "../player/Lives.h"

UIManager::UIManager(EventManager& eventManager):
   eventManager(eventManager) {}

void UIManager::drawHUD(int score){
   std::string str = std::to_string(score);
   const char* ch = str.c_str();
   DrawText(ch, 1, 1, 20, WHITE);
}
 
void UIManager::drawTime(float deltaTime){

   const char* ch = TextFormat("Time: %.1f", deltaTime);
   DrawText(ch, GetScreenWidth()/2 - MeasureText(ch,4) , 1 ,20,WHITE);
}

void UIManager::drawMainMenu(){
   float w=160, h=40;
   float centerX = (float)GetScreenWidth()/2 - w/2;
   

    GuiButton({centerX, (float)GetScreenHeight()/4   ,w,h}, "Dynamic Maze");
    GuiButton({centerX, (float)GetScreenHeight()/4+8 ,w,h}, "Dark Maze");
    GuiButton({centerX, (float)GetScreenHeight()/4+16,w,h}, "Gauntlet");
    GuiButton({centerX, (float)GetScreenHeight()/4+24,w,h}, "Settings");
   
}


void UIManager::drawGameOver(int score){
   int sw = GetScreenWidth();
   int sh = GetScreenHeight();

   DrawText("GAME OVER", sw/2-MeasureText("GAME OVER", 40)/2 , sh/3 , 40,RED);

   std::string str = "Score"+std::to_string(score);
   const char* ch = str.c_str();

   DrawText(ch, sw/2 - MeasureText(ch, 24), sh/3+60, 24, WHITE);

   if(GuiButton({(float)sw/2-80, (float)sh/3+120, 160,40}, "Retry"))
      selectedIndex=10;
   if(GuiButton({(float)sw/2-80, (float)sh/3+170, 160,40}, "Main Menu"))
      selectedIndex=0;

}

void UIManager::drawLevelComplete(int score){
   int sw = GetScreenWidth();
   int sh = GetScreenHeight();

   DrawText("LEVEL COMPLETE", sw/2-MeasureText("LEVEL COMPLETE", 36)/2 , sh/3 , 36,GREEN);

   std::string str = "Score"+std::to_string(score);
   const char* ch = str.c_str();

   DrawText(ch, sw/2 - MeasureText(ch, 24), sh/3+60, 24, WHITE);

   if(GuiButton({(float)sw/2-80, (float)sh/3+120, 160,40}, "Next Level"))
      selectedIndex=20;
   if(GuiButton({(float)sw/2-80, (float)sh/3+170, 160,40}, "Main Menu"))
      selectedIndex=0;

}


void UIManager::drawSettings() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawText("SETTINGS", sw/2 - MeasureText("SETTINGS", 32)/2, sh/4, 32, WHITE);

    DrawText("Volume", sw/2 - 120, sh/4 + 80, 20, WHITE);
    GuiSlider({(float)sw/2 - 60, (float)sh/4 + 80, 160, 30}, "0", "100", &volume, 0.0f, 100.0f);

    DrawText("Difficulty", sw/2 - 120, sh/4 + 130, 20, WHITE);
    if (GuiButton({(float)sw/2 - 60, (float)sh/4 + 130, 70, 30}, "Easy"))   difficulty = 0;
    if (GuiButton({(float)sw/2 + 20, (float)sh/4 + 130, 70, 30}, "Hard"))   difficulty = 1;

    if (GuiButton({(float)sw/2 - 80, (float)sh/4 + 200, 160, 40}, "Back"))
        selectedIndex = 0;
}

void UIManager::drawLives(const Lives& lives){
   int sw = GetScreenWidth();
   for(int i=0; i<lives.getCount(); i++){
      DrawCircle(sw-20-(i*25),20,8,RED);
   }
}


