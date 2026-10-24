#include "GridMazeMutator.h"
#include "../../core/Profiling.h"

std::vector<std::pair<Wall*,float>> GridMazeMutator::selectWalls(Maze& maze, const Position& playerPos, const Position& exitPos){
   std::vector<std::pair<Wall*,float>> result;
   std::uniform_real_distribution<float> dis(0.0f, 1.0f);

   for(const auto& w:maze.getWalls()){
      Wall* wall = w.get();
      float chanceTo = wall->getIsOpen()?CLOSE_PROBABILITY:OPEN_PROBABILITY;
      if(dis(this->gen)>chanceTo) continue;      
      auto[r1,c1] = wall->getCells().first;
      auto[r2,c2] = wall->getCells().second;
      Position p1(r1,c1), p2(r2,c2);
      
      if (p1 == playerPos || p2 == playerPos || p1 == exitPos || p2 == exitPos) continue;

      Position mid( (r1+r2)/2.0f, (c1+c2)/2.0f );
      float d = playerPos.distanceTo(mid);

      if(d>= MIN_MUTATE_DIST && d<MAX_MUTATE_DIST)
         result.push_back({wall,d});
   }
   return result;
}

void GridMazeMutator::queueWall(Wall *wall, float dist){
   bool willOpen = wall->getIsOpen();
   float delay = std::clamp((dist - MIN_MUTATE_DIST) * DELAY_FACTOR + MIN_DELAY, MIN_DELAY, MAX_DELAY);
    
   pendingQueue.push_back({willOpen, delay, wall});
    
   auto cells = wall->getCells();
   eventManager.publish<WallTelegraphEvent>({
      cells.first.first, cells.first.second,
      cells.second.first, cells.second.second,
      willOpen, delay
   });
}

std::vector<std::pair<Wall*,float>> GridMazeMutator::applyWindow(std::vector<std::pair<Wall*,float>>& walls, int idx, Maze& maze){
   std::vector<std::pair<Wall*,float>> currentBatch;
   for(int i=idx; i<std::min(idx+batchSize,(int)walls.size()); i++){
      Wall* it = walls[i].first;
      if(it->getIsOpen()){
         if(canSafelyclose(maze, it)){
            it->close();
            currentBatch.push_back(walls[i]);
         }
      }else{
         if(!createOpenBlock(maze, it)){
            it->open();
            currentBatch.push_back(walls[i]);
         }
      }
   }
   return currentBatch;
}

void GridMazeMutator::publishChange(Wall* wall){
   auto cell = wall->getCells();
   eventManager.publish<WallStateChangedEvent>({
            cell.first.first, cell.first.second,
            cell.second.first, cell.second.second, wall->getIsOpen()
         });
}

bool GridMazeMutator::tryMutateWall(Maze& maze, Wall* wall, const Position& playerPos, const Position& exitPos){
   wall->getIsOpen()? wall->close():wall->open();
   if(wall->getIsOpen()) return true;
   if(checker.isSolvable(maze, playerPos, exitPos)){
      return true;
   }
   rollbackWall(wall);
   return false;
}

void GridMazeMutator::rollbackWall(Wall* wall){
   wall->getIsOpen()?wall->close():wall->open();
}

void GridMazeMutator::mutate(Maze& maze,const Position& playerPos,const Position& exitPos){
   PROFILE_SCOPE;
   if(!pendingQueue.empty()) return;
   
   auto selected = selectWalls(maze,playerPos, exitPos);
   if(selected.empty()) return;
   std::sort(selected.begin(), selected.end(), [](const auto& a, const auto&b){
      return a.second < b.second;
   });

   int n = selected.size();
   std::vector<std::pair<Wall*,float>> plannedChanges;

   for(int i=0; i<n; i+=batchSize){
      auto currentBatch = applyWindow(selected, i, maze);
      if(currentBatch.empty()) continue;

      if(checker.isSolvable(maze,playerPos, exitPos)){
         for(auto& pair:currentBatch) plannedChanges.push_back(pair);
      }else{
         for(auto& pair:currentBatch) rollbackWall(pair.first);
         for(auto& pair:currentBatch){
            if(tryMutateWall(maze, pair.first, playerPos, exitPos)) plannedChanges.push_back(pair);
         }
      }
   }
   for(auto& pair:plannedChanges){
      queueWall(pair.first, pair.second);
      rollbackWall(pair.first);
   }
}

void GridMazeMutator::update(float deltaTime, Maze &maze, const Position &playerPos, const Position &exitPos){
   if(pendingQueue.empty()) return;
   for(auto& pm:pendingQueue) pm.timeLeft-=deltaTime;

   std::vector<PendingMutation*> due;
   for(auto& pm:pendingQueue){
      if(pm.timeLeft<=0.0f) due.push_back(&pm);
   }
   if(due.empty()) return;

   std::sort(due.begin(), due.end(), [](const auto*a, const auto*b){
      return a->willOpen > b->willOpen;
   });

   for(auto* pm:due){
      auto[c1,c2] = pm->wall->getCells();
      Position p1(c1.first, c1.second), p2(c2.first,c2.second);
      if(p1==playerPos || p2==playerPos || p1==exitPos || p2==exitPos) continue;

      if(pm->willOpen){
         pm->wall->open();
         publishChange(pm->wall);
      }else{
         pm->wall->close();
         if(checker.isSolvable(maze, playerPos, exitPos)){
            publishChange(pm->wall);
         }else{
            std::printf("Commit cancelled by BFS re-check!\n");
            pm->wall->open();
         }
      }

   }
   std::erase_if(pendingQueue, [](const auto& pm) { return pm.timeLeft <= 0.0f; });
}

bool GridMazeMutator::createOpenBlock(Maze &maze, Wall *wall){
auto [c1, c2] = wall->getCells();
   int r1 = c1.first, col1 = c1.second;
   int r2 = c2.first, col2 = c2.second;

   if (r1 > r2 || (r1 == r2 && col1 > col2)) {
      std::swap(r1, r2);
      std::swap(col1, col2);
   }

   auto isOpen = [&](int r, int c, DirectionType dir) {
      Cell* cell = maze.getCell(r, c);
      return cell && cell->hasOpenWall(dir);
   };

   if (r1 == r2) {
      if (isOpen(r1, col1, DirectionType::UP) && 
         isOpen(r2, col2, DirectionType::UP) && 
         isOpen(r1 - 1, col1, DirectionType::RIGHT)) {
         return true;
      }
      if (isOpen(r1, col1, DirectionType::DOWN) && 
         isOpen(r2, col2, DirectionType::DOWN) && 
         isOpen(r1 + 1, col1, DirectionType::RIGHT)) {
         return true;
      }
   }else if (col1 == col2) {
      if (isOpen(r1, col1, DirectionType::LEFT) && 
         isOpen(r2, col2, DirectionType::LEFT) && 
         isOpen(r1, col1 - 1, DirectionType::DOWN)) {
         return true;
      }
      if(isOpen(r1, col1, DirectionType::RIGHT) && 
         isOpen(r2, col2, DirectionType::RIGHT) && 
         isOpen(r1, col1 + 1, DirectionType::DOWN)) {
         return true;
      }
   }
   return false;
}

bool GridMazeMutator::canSafelyclose(Maze &maze, Wall *wall){
   auto [c1,c2] = wall->getCells();
   Cell* cell1 = maze.getCell(c1.first, c1.second);
   Cell* cell2 = maze.getCell(c2.first, c2.second);

   int cnt1=0, cnt2=0;

   for(DirectionType dir:ALL_DIRECTIONS){
      if(cell1->hasOpenWall(dir)) cnt1++;
      if(cell2->hasOpenWall(dir)) cnt2++;
   }
   if(cnt1<=1 || cnt2<=1) return false;
   return true;

}