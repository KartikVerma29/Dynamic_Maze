#include "Player.h"

Player::Player(Position& position, Direction& direction): position(position), direction(direction){}

void Player::onEvent(const PlayerHitEvent& event){

}

void Player::onEvent(const PlayerMovedEvent& event){
   position = event.newPosition;
   direction = event.direction;
}

void Player::onEvent(const WallStateChangedEvent& event){
   if(!event.isOpen){

   }
}
