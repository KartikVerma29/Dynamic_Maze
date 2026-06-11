#include "Patrol.h"

void Patrol::onEvent(const WallStateChangedEvent& event){
   if(cachedPath.empty()) return;
   if(event.isOpen) return;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

void Patrol::onEvent(const PlayerMovedEvent& event){
   lastPlayerDirection = event.direction;
}

bool Patrol::isDefeatable(const Direction& direction)const{
   return lastPlayerDirection==direction;
}

void Patrol::update(Maze& maze, float deltaTime) {
    if (isDefeated) return;

    if (cachedPath.empty() || pathIndex >= (int)cachedPath.size()) {
        Cell* current = maze.getCell(position.getX(), position.getY());
        if (!current) return;

        std::vector<Position> options;
        for (auto dir : {DirectionType::UP, DirectionType::DOWN,
                         DirectionType::LEFT, DirectionType::RIGHT}) {
            Wall* wall = current->getWall(dir);
            if (wall && wall->getIsOpen()) {
                Direction d(dir);
                Position offset = d.toVector();
                options.push_back(Position(
                    position.getX() + offset.getX(),
                    position.getY() + offset.getY()
                ));
            }
        }

        if (options.empty()) return;

        cachedPath.clear();
        pathIndex = 0;
        cachedPath.push_back(options[rand() % options.size()]);
    }

    if(!cachedPath.empty() && pathIndex<(int)cachedPath.size()){
        position = cachedPath[pathIndex];
        direction = Direction(DirectionType::UP);
        pathIndex++;
    }
}
