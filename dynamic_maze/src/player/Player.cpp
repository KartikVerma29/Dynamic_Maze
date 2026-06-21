#include "Player.h"

Player::Player(Position position, Direction direction): position(position), direction(direction), visual(position){}

void Player::onEvent(const PlayerHitEvent& event){

}

void Player::onEvent(const PlayerMovedEvent& event){
   if(!maze) return;
   Direction d = event.direction;
   Position offset = d.toVector();
   
   int newRow = position.getX() + offset.getX();
   int newCol = position.getY() + offset.getY();

   Cell* current = maze->getCell(position.getX(), position.getY()); 
   if(!current) return;
   direction = event.direction;

   Wall* wall = current->getWall(event.direction.getType());
   if(wall && wall->getIsOpen()){
      position.setX(newRow);
      position.setY(newCol);
      visual.setTag(position);
   }

}

void Player::onEvent(const WallStateChangedEvent& event){
   if(!event.isOpen){

   }
}
