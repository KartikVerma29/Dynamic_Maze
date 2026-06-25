#pragma once
#include "Position.h"

struct FloatPos{ float x; float y;};

class VisualComponent{
private:
   Position prevPos;
   Position tagPos;
   FloatPos renderPos;
   float moveProgess=1.0f;
   float moveSpeed;
public:
   VisualComponent(Position startPosition, float moveSpeed = 5.0f);
   void setTag(const Position& newTag);
   void update(float deltaTime);
   FloatPos getRenderPosition() const{ return renderPos; }
   float getProgress() const {return moveProgess;}
};
