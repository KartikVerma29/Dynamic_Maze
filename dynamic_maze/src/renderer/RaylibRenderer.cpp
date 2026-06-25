#include "RaylibRenderer.h"
#include <raylib.h>
#include <raymath.h>


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
   mazeCols = maze.getCols();
   mazeRows = maze.getRows();
   cellSize = (1.0f*std::min(height, width))/(1.0f*std::max(mazeCols, mazeRows));

   DrawLine(0,0,maze.getCols()*cellSize,0,WHITE);
   DrawLine(0,0,0,maze.getRows()*cellSize, WHITE);  
   DrawLine(0,maze.getCols()*cellSize,mazeCols*cellSize,maze.getRows()*cellSize, WHITE);  
   DrawLine(mazeRows*cellSize,0,mazeCols*cellSize,maze.getRows()*cellSize, WHITE);  

   for(int r=0; r<maze.getRows(); r++){
      for(int c=0; c<maze.getCols(); c++){
         Cell* cell = maze.getCell(r, c);
         if(cell) drawCell(*cell);
      }
   }
}

void RaylibRenderer::drawPlayer(Player& player){
   float playerSize = cellSize/2 - 0.1f;
   
   FloatPos playerPos = player.getRenderPosition();
   Direction dir = player.getDirection();

   float px = playerPos.y * cellSize + cellSize * 0.5f;  // col → x
   float py = playerPos.x * cellSize + cellSize * 0.5f;  // row → y
   Vector2 center = {px,py};
   DrawCircleV(center, cellSize/2, SKYBLUE);
   
   Vector2 tip = Vector2Add(center, Vector2Scale({dir.toVector().getY(),dir.toVector().getX()}, cellSize/2.0f));
   DrawLineEx(center, tip, 3.0f,WHITE);
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
   float py = enemyPos.getX()* cellSize + cellSize * 0.5f;  // row → y
   DrawCircle(px,py,enemySize, colr);

//    Vector2 center = {px,py};
//    DrawCircleV(center, cellSize/3.0f, colr);
//    Direction dir = ienemy.getDirection();

//    Vector2 tip = Vector2Add(center, Vector2Scale({dir.toVector().getY(), dir.toVector().getX()},cellSize/2.0f));
//    DrawLineEx(center,tip,3.0f,BLACK);

}

void RaylibRenderer::drawFog(Position& position, float radius) {
    int screenW = mazeCols*cellSize;
    int screenH = GetScreenHeight();
    int tilesX = mazeCols;
    int tilesY = mazeRows;

    if (fogTexture.id == 0 || fogTexture.texture.width != tilesX) {
        if (fogTexture.id != 0) UnloadRenderTexture(fogTexture);
        fogTexture = LoadRenderTexture(tilesX, tilesY);

        SetTextureFilter(fogTexture.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(fogTexture.texture, TEXTURE_WRAP_CLAMP);
    }

    float px = position.getY() * cellSize + cellSize * 0.5f;
    float py = position.getX() * cellSize + cellSize * 0.5f;
    float pixelRadius = radius * cellSize * 1.5f;

    BeginTextureMode(fogTexture);
        ClearBackground(Color{ 0,0,0,0}); 
        // DrawCircleGradient(Vector2(px,py), pixelRadius*0.8, WHITE, Color{ 10, 10, 10, 255 });

        for(int r = 0; r <= mazeRows; r++){
            for(int c = 0; c <= mazeCols; c++){
                float dist = Position(r, c).distanceTo(position); 
                // int rectX = c * (int)cellSize;
                // int rectY = r * (int)cellSize;
                // int size = (int)cellSize;

                float fadeStart = radius-2.5f;
                if(dist > radius) {
                    DrawRectangle(c, r, 1, 1, BLACK);
                }
                else if(dist > fadeStart) {
                    float fadeRatio = (dist - fadeStart)/(radius-fadeStart);
                    unsigned char alpha = (unsigned char)(fadeRatio*255.0f);
                    DrawRectangle(c, r, 1, 1, Color{0, 0, 0, alpha});
                }
            }
        }
    EndTextureMode();

    BeginBlendMode(BLEND_ALPHA);
    // DrawTextureRec(
    //     fogTexture.texture,
    //     { 0, 0, (float)fogTexture.texture.width, (float)-fogTexture.texture.height },
    //     { 0, 0 },
    //     WHITE
    // );
    DrawTexturePro(
        fogTexture.texture,
        {0,0,(float)fogTexture.texture.width, (float)-fogTexture.texture.height},
        {0,0,(float)tilesX*cellSize, (float)tilesY*cellSize},
        {0,0} ,0.0f, WHITE
    );
    EndBlendMode();
}
