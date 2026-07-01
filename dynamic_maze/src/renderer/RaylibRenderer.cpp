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
    if(mazeTexture.id != 0) UnloadRenderTexture(mazeTexture);
    CloseWindow();
}

void RaylibRenderer::beginFrame(){ BeginDrawing(); }

void RaylibRenderer::endFrame(){ EndDrawing(); }

void RaylibRenderer::clearScreen(){ ClearBackground(BLACK); }

void RaylibRenderer::drawCell(Cell& cell){
   int px = cell.getCol()*INTERNAL_CELL_SIZE;
   int py = cell.getRow()*INTERNAL_CELL_SIZE;
   int th = WALL_THICKNESS;

   Wall* right = cell.getWall(DirectionType::RIGHT);
   if(right && !right->getIsOpen()){
        DrawRectangle(px+INTERNAL_CELL_SIZE-th, py-th, th, INTERNAL_CELL_SIZE+th, WHITE);
   }

   Wall* down = cell.getWall(DirectionType::DOWN);
   if(down && !down->getIsOpen()){
        DrawRectangle(px-th, py+INTERNAL_CELL_SIZE-th, INTERNAL_CELL_SIZE+th, th, WHITE);
   }

}

void RaylibRenderer::drawMaze(Maze& maze){
   mazeCols = maze.getCols();
   mazeRows = maze.getRows();
   cellSize = (1.0f*std::min(height, width))/(1.0f*std::max(mazeCols, mazeRows));

   int texWidth = mazeCols*INTERNAL_CELL_SIZE;
   int texHeight = mazeRows*INTERNAL_CELL_SIZE;

   if(mazeTexture.id==0 || mazeTexture.texture.width!=texWidth || mazeTexture.texture.height!=texHeight){
        if(mazeTexture.id!=0) UnloadRenderTexture(mazeTexture);
        mazeTexture = LoadRenderTexture(texWidth, texHeight);
        SetTextureFilter(mazeTexture.texture, TEXTURE_FILTER_POINT);
   }

   BeginTextureMode(mazeTexture);
   ClearBackground(BLACK);
        for(int r=0; r<maze.getRows(); r++){
            for(int c=0; c<maze.getCols(); c++){
                Cell* cell = maze.getCell(r, c);
                if(cell) drawCell(*cell);
            }
        }

        int th = WALL_THICKNESS;
        DrawRectangle(0, 0, texWidth, th, WHITE);
        DrawRectangle(0, 0, th, texHeight, WHITE);
        DrawRectangle(0, texHeight - th, texWidth, th, WHITE);
        DrawRectangle(texWidth - th, 0, th, texHeight, WHITE);
    EndTextureMode();

    Rectangle source = { 0.0f, 0.0f, (float)mazeTexture.texture.width, (float)-mazeTexture.texture.height };
    Rectangle dest = { 0.0f, 0.0f, (float)(mazeCols * cellSize), (float)(mazeRows * cellSize) };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(mazeTexture.texture, source, dest, origin, 0.0f, WHITE);
}

void RaylibRenderer::drawPlayer(Player& player){
   float playerSize = cellSize/2 - 0.1f;
   
   FloatPos playerPos = player.getRenderPosition();
   Direction dir = player.getDirection();

   float px = playerPos.y * cellSize + cellSize * 0.5f;  // col → x
   float py = playerPos.x * cellSize + cellSize * 0.5f;  // row → y
   Vector2 center = {px,py};
   DrawCircleV(center, cellSize/2-WALL_THICKNESS*2, SKYBLUE);
   
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

   float enemySize = cellSize/2-0.2f-WALL_THICKNESS*2;
   FloatPos enemyPos =ienemy.getRenderPosition();
   float px = enemyPos.y* cellSize + cellSize * 0.5f;  // col → x
   float py = enemyPos.x* cellSize + cellSize * 0.5f;  // row → y
   DrawCircle(px,py,enemySize, colr);

   Vector2 center = {px,py};
   DrawCircleV(center, cellSize/3.0f, colr);
   Direction dir = ienemy.getDirection();

   Vector2 tip = Vector2Add(center, Vector2Scale({dir.toVector().getY(), dir.toVector().getX()},cellSize/2.0f));
   DrawLineEx(center,tip,3.0f,BLACK);

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

    if(position.distanceTo(lastFogPos)>0.01f){
        lastFogPos=position;
        
        BeginTextureMode(fogTexture);
        ClearBackground(Color{ 0,0,0,0}); 
        float fadeStart = radius-2.5f;
        
        for(int r = 0; r <= mazeRows; r++){
            for(int c = 0; c <= mazeCols; c++){
                float dist = Position(r, c).distanceTo(position); 
                
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
    }

    BeginBlendMode(BLEND_ALPHA);
    DrawTexturePro(
        fogTexture.texture,
        {0,0,(float)fogTexture.texture.width, (float)-fogTexture.texture.height},
        {0,0,(float)tilesX*cellSize, (float)tilesY*cellSize},
        {0,0} ,0.0f, WHITE
    );
    EndBlendMode();
}
