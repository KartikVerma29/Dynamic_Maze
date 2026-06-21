#pragma once
#include "Position.h"
enum class DirectionType {UP, DOWN, LEFT, RIGHT};

class Direction{
   private:
   DirectionType type;
public:
   Direction(): type(DirectionType::UP){}
   Direction(DirectionType type):type(type){}


   DirectionType getType() const{
      return this->type;
   }
   Direction opposite() const{
      switch(this->type) {
         case DirectionType::UP: return Direction(DirectionType::DOWN);
         case DirectionType::DOWN: return Direction(DirectionType::UP);
         case DirectionType::LEFT: return Direction(DirectionType::RIGHT);
         case DirectionType::RIGHT: return Direction(DirectionType::LEFT);
      }
   }

   Position toVector() const{
      switch(this->type) {
         case DirectionType::UP: return Position(-1,0);
         case DirectionType::DOWN: return Position(1,0);
         case DirectionType::LEFT: return Position(0,-1);
         case DirectionType::RIGHT: return Position(0,1);
      }
   }
   Direction rotate90() const{
     switch(type) {
        case DirectionType::UP: return Direction(DirectionType::RIGHT);
        case DirectionType::RIGHT: return Direction(DirectionType::DOWN);
        case DirectionType::DOWN: return Direction(DirectionType::LEFT);
        case DirectionType::LEFT: return Direction(DirectionType::UP);
     } 
   }

   Direction rotateTo(int angle) const;

   bool operator==(const Direction& other) const{
      return type == other.type;
   }
   
   float toAngle() const{
      switch(type){
         case DirectionType::UP: return 0.0f; 
         case DirectionType::RIGHT: return 90.0f;
         case DirectionType::DOWN: return 180.0f;
         case DirectionType::LEFT: return 270.0f;
      }
      return 0.0f;
   }

};
