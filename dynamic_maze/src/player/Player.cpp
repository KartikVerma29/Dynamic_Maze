#include "Player.h"

Player::Player(Position position, Direction direction): position(position), direction(direction){}

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

   Wall* wall = current->getWall(event.direction.getType());
   if(wall && wall->getIsOpen()){
      position.setX(newRow);
      position.setY(newCol);
      direction = event.direction;
   }

}

void Player::onEvent(const WallStateChangedEvent& event){
   if(!event.isOpen){

   }
}
