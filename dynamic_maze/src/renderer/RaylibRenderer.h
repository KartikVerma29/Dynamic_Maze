#pragma once
#include "IRenderer.h"
#include "../events/events/WallTelegraphEvent.h"
#include "../events/IEventListener.h"
#include <raylib.h>
#include <raymath.h>
#include "../core/Profiling.h"

class RaylibRenderer:public IRenderer, public IEventListener<WallTelegraphEvent>
{
private:
   int width, height;
   int cellSize;

   RenderTexture2D fogTexture;
   Position lastFogPos = Position(-100.0f, -100.0f);

   RenderTexture2D gameTexture;
   const int WALL_THICKNESS = 2;
   int mazeRows, mazeCols;

   struct ActiveTelegraph{
      int r1,c1,r2,c2;
      bool willOpen;
      float totalDelay, timeLeft;
   };
   std::vector<ActiveTelegraph> activeTelegraphs;

public:
   RaylibRenderer(int width, int height, const char* title, int fps=60);
   void beginFrame() override;
   void endFrame() override;
   void clearScreen() override;

   void beginWorld() override;
   void endWorld() override;

   void drawCell(Cell &cell) override;
   void drawEnemy(IEnemy &ienemy) override;
   void drawFog() override;
   void updateFog(Position& position, float radius) override;
   void drawMaze(Maze &maze) override;
   void drawPlayer(Player &player) override;
   void setMazeDimensions(int rows, int cols) override{
      mazeCols=cols; mazeRows=rows;
   }
   void onEvent(const WallTelegraphEvent& event) override;
   void update(float deltaTime) override;
   void drawTelegraphs() override;
   void clearTelegraphs() override{ activeTelegraphs.clear();};
   void drawExit(const Position& exitPos) override;
   ~RaylibRenderer();
};

