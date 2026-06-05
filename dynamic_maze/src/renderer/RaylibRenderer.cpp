#include "RaylibRenderer.h"
#include "IRenderer.h"


RaylibRenderer::RaylibRenderer(int width, int height, const char* title):
   width(width), height(height), title(title){
      InitWindow(width,height,title);
      SetTargetFPS(60);
      fogTexture = LoadRenderTexture(mazeCols, mazeRows);
      SetTextureFilter(fogTexture.texture, TEXTURE_FILTER_BILINEAR);
      SetTextureWrap(fogTexture.texture, TEXTURE_WRAP_CLAMP);
}

RaylibRenderer::~RaylibRenderer(){
   UnloadRenderTexture(fogTexture);
   CloseWindow();
}

void RaylibRenderer::beginFrame(){ BeginDrawing(); }

void RaylibRenderer::endFrame(){ EndDrawing(); }

void RaylibRenderer::clearScreen(){ ClearBackground(BLACK); }

void RaylibRenderer::drawCell(Cell& cell){
   float pixelX = cell.getCol()*cellSize;
   float pixelY = cell.getRow()*cellSize;
   DrawRectangle(cell.getCol(), cell.getRow(), pixelX, pixelY, GRAY);

   for(auto dir:{DirectionType::DOWN, DirectionType::RIGHT}){
      Wall* wall = cell.getWall(dir);
      if(wall && !wall->getIsOpen()){
         DrawLine(pixelX+cellSize, pixelY, pixelX+cellSize, pixelY+cellSize, WHITE);
      }
   }

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
   DrawCircle(playerPos.getX(), playerPos.getY(), playerSize, SKYBLUE);
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
   DrawCircle(enemyPos.getX(), enemyPos.getY(), enemySize, colr);
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


