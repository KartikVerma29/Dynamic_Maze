#pragma once
#include "../core/Cell.h"
#include "../maze/Maze.h"
#include "../enemy/IEnemy.h"
#include "../player/Player.h"

class IRenderer{
public:
   virtual void beginFrame()=0;
   virtual void endFrame()=0;
   virtual void clearScreen()=0;

   virtual void beginWorld() = 0;
   virtual void endWorld() = 0;

   virtual void drawCell(Cell& cell)=0;
   virtual void drawPlayer(Player& player)=0;
   virtual void drawEnemy(IEnemy& ienemy)=0;
   virtual void drawMaze(Maze& maze)=0;
   virtual void updateFog(Position& position, float radius)=0;
   virtual void drawFog() = 0;
   virtual void setMazeDimensions(int rows, int cols) = 0;
   virtual ~IRenderer()=default;
};
