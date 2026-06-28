#pragma once
#include "IRenderer.h"
#include <raylib.h>

class RaylibRenderer:public IRenderer
{
private:
   int width, height;
   const char* title;
   float cellSize;
   RenderTexture2D fogTexture={0};
   Position lastFogPos = Position(-100.0f, -100.0f);
   int mazeRows, mazeCols;
public:
   RaylibRenderer(int width, int height, const char* title);
   void beginFrame() override;
   void endFrame() override;
   void clearScreen() override;
   void drawCell(Cell &cell) override;
   void drawEnemy(IEnemy &ienemy) override;
   void drawFog(Position& position, float radius) override;
   void drawMaze(Maze &maze) override;
   void drawPlayer(Player &player) override;
   void setMazeDimensions(int rows, int cols){
      mazeCols=cols; mazeRows=rows;
   }
   ~RaylibRenderer();
};

