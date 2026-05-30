#pragma once
#include <cmath>

class Position{
private:
   float x,y;
public:
   Position():x(0),y(0){}
   Position(float x, float y): x(x), y(y){}

   float getX() const{ return this->x;}
   float getY() const{ return this->y;}
   void setX(float x){ this->x=x;}
   void setY(float y){ this->y=y;}
   float distanceTo(const Position& other) const{
      float dx= this->x - other.x;
      float dy= this->y - other.y;
      return std::sqrt(dx*dx + dy*dy);
   }
   bool operator==(const Position& other) const{
      return this->x==other.x && this->y==other.y;
   }

};

