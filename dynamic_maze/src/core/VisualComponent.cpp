#include "VisualComponent.h"

VisualComponent::VisualComponent(Position startPosition, float moveSpeed):
   prevPos(startPosition), tagPos(startPosition), moveSpeed(moveSpeed) {
      renderPos.x = (float) startPosition.getX();
      renderPos.y = (float) startPosition.getY();
   }

void VisualComponent::setTag(const Position& newTag){
   prevPos = tagPos;
   tagPos = newTag;
   moveProgess=0.0f;
}

void VisualComponent::update(float deltaTime){
   if(moveProgess>=1.0f) return;
   moveProgess+=deltaTime*moveSpeed;

   if(moveProgess>1.0f) moveProgess=1.0f;

   renderPos.x = prevPos.getX() + (tagPos.getX() - prevPos.getX())*moveProgess;
   renderPos.y = prevPos.getY() + (tagPos.getY() - prevPos.getY())*moveProgess;
}

