#pragma once
#include "Position.h"
#include <array>
#include <cstddef>
#include <cstdint>
enum class DirectionType:uint8_t {UP=0, RIGHT=1, DOWN=2, LEFT=3};

inline constexpr std::array<DirectionType, 4> ALL_DIRECTIONS = {
   DirectionType::UP, DirectionType::RIGHT, DirectionType::DOWN, DirectionType::LEFT
};

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
      return Direction(static_cast<DirectionType>(static_cast<int>(type)^2));
   }

   Position toVector() const{
      static const std::array<Position,4> vec={
         Position(-1,0), Position(0,1), Position(1,0), Position(0,-1)
      };
      return vec[static_cast<size_t>(type)];
   }
   Direction rotate90() const{
      return Direction(static_cast<DirectionType>((static_cast<int>(type)+3)&3));
   }

   Direction rotateTo(int angle) const {
      int normalizedAngle = ((angle % 360) + 360) % 360;
      
      if (normalizedAngle >= 45 && normalizedAngle < 135) return Direction(DirectionType::RIGHT);
      if (normalizedAngle >= 135 && normalizedAngle < 225) return Direction(DirectionType::DOWN);
      if (normalizedAngle >= 225 && normalizedAngle < 315) return Direction(DirectionType::LEFT);
      
      return Direction(DirectionType::UP);
   }

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
