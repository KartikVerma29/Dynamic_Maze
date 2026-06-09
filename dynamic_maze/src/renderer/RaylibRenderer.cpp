#include "RaylibRenderer.h"
#include "IRenderer.h"


RaylibRenderer::RaylibRenderer(int width, int height, const char* title):
   width(width), height(height), title(title){
      InitWindow(width,height,title);
      SetTargetFPS(60);
      
}

RaylibRenderer::~RaylibRenderer() {
    if (fogTexture.id != 0) UnloadRenderTexture(fogTexture);
    CloseWindow();
}

void RaylibRenderer::beginFrame(){ BeginDrawing(); }

void RaylibRenderer::endFrame(){ EndDrawing(); }

void RaylibRenderer::clearScreen(){ ClearBackground(BLACK); }

void RaylibRenderer::drawCell(Cell& cell){
   float pixelX = cell.getCol()*cellSize;
   float pixelY = cell.getRow()*cellSize;
   DrawRectangle((int)pixelX, (int)pixelY, (int)cellSize, (int)cellSize, BLACK);

   Wall* right = cell.getWall(DirectionType::RIGHT);
   if(right && !right->getIsOpen())
      DrawLine((int)(pixelX+cellSize), (int)pixelY,
             (int)(pixelX+cellSize), (int)(pixelY+cellSize), WHITE);

   Wall* down = cell.getWall(DirectionType::DOWN);
   if(down && !down->getIsOpen())
      DrawLine((int)pixelX,            (int)(pixelY+cellSize),
             (int)(pixelX+cellSize), (int)(pixelY+cellSize), WHITE);

}

void RaylibRenderer::drawMaze(Maze& maze){
   cellSize = (1.0f*std::min(height, width))/(1.0f*std::max(maze.getCols(), maze.getRows()));

   DrawLine(0,0,maze.getCols()*cellSize,0,WHITE);
   DrawLine(0,0,0,maze.getRows()*cellSize, WHITE);

   for(int r=0; r<maze.getRows(); r++){
      for(int c=0; c<maze.getCols(); c++){
         Cell* cell = maze.getCell(r, c);
         if(cell) drawCell(*cell);
      }
   }
}

void RaylibRenderer::drawPlayer(Player& player){
   float playerSize = cellSize/2 - 0.1f;
   Position playerPos = player.getPosition();
   float px = playerPos.getY() * cellSize + cellSize * 0.5f;  // col → x
   float py = playerPos.getX() * cellSize + cellSize * 0.5f;  // row → y
   DrawCircle((int)px, (int)py, playerSize, SKYBLUE);
}

void RaylibRenderer::drawEnemy(IEnemy& ienemy){
   Color colr;
   switch (ienemy.getType()) {
      case EnemyType::CHASER  :{ colr=RED; break;}
      case EnemyType::BLOCKER :{ colr=YELLOW; break;}
      case EnemyType::PATROL  :{ colr=ORANGE; break;}
   }

   float enemySize = cellSize/2-0.2f;
   Position enemyPos =ienemy.getPosition();
   float px = enemyPos.getY() * cellSize + cellSize * 0.5f;  // col → x
   float py = enemyPos.getX() * cellSize + cellSize * 0.5f;  // row → y
   DrawCircle(px,py,enemySize, colr);
}


/*
 // Fog of war technique based on raylib example
// Copyright (c) 2018-2025 Ramon Santamaria (@raysan5)
// Licensed under zlib/libpng license
 
 */

void RaylibRenderer::drawFog(Position& position, float radius){
   int tileSize = cellSize;
   int tilesX = mazeCols;
   int tilesY = mazeRows;

   if (fogTexture.id == 0 || mazeCols != fogTexture.texture.width) {
        if (fogTexture.id != 0) UnloadRenderTexture(fogTexture);
        fogTexture = LoadRenderTexture(mazeCols, mazeRows);
        SetTextureFilter(fogTexture.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(fogTexture.texture, TEXTURE_WRAP_CLAMP);
    }


   BeginTextureMode(fogTexture);
      ClearBackground(BLANK);
      for(int y=0; y<tilesY; y++){
         for(int x=0; x<tilesX; x++){
            float dist = Position(x,y).distanceTo(position);
            if(dist>radius)
               DrawRectangle(x,y, 1, 1, BLANK);
            else if(dist>radius*0.6f)
               DrawRectangle(x,y,1,1,Fade(BLANK,0.6f));
         }

      }
      EndTextureMode();

   DrawTexturePro(
      fogTexture.texture,
      {0,0, (float)fogTexture.texture.width, (float)-fogTexture.texture.height},
      {0,0, (float)tilesX*tileSize, (float)tilesY*tileSize},
      {0,0,}, 0.0f, WHITE
   );
}


