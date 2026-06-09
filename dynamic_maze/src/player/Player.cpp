#include "Player.h"

Player::Player(Position position, Direction direction): position(position), direction(direction){}

void Player::onEvent(const PlayerHitEvent& event){

}

void Player::onEvent(const PlayerMovedEvent& event){
   Direction d = event.direction;
   Position offset = d.toVector();
   position.setX(position.getX() + offset.getX());
   position.setY(position.getY() + offset.getY());
   direction = event.direction;
}

void Player::onEvent(const WallStateChangedEvent& event){
   if(!event.isOpen){

   }
}
