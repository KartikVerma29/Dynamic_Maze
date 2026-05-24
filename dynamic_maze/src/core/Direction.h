#pragma once
#include "Position.h"

class Direction{
public:
   enum class DirectionType {UP, DOWN, LEFT, RIGHT};
private:
   DirectionType type;
public:
   
   DirectionType getType() const;
   Direction opposite() const;
   Position toVector() const;
   Direction rotate90() const;
   Direction rotateTo(int angle) const;
   bool operator==(const Direction& other) const;

};
