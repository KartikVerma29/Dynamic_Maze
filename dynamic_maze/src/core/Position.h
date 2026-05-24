#pragma once
class Position{
private:
   float x,y;
public:
   Position(float x, float y);

   float getX() const;
   float getY() const;
   void setX(float x);
   void setY(float y);
   float distanceTo(const Position& other) const;
   bool operator==(const Position& other) const;

};
