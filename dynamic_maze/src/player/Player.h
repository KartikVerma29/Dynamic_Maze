#pragma once
#include "../events/IEventListener.h"
#include "../events/events/PlayerMovedEvent.h"
#include "../events/events/PlayerHitEvent.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../core/Position.h"
#include "../core/Direction.h"
#include "../maze/Maze.h"
#include "../core/VisualComponent.h"


class Player: public IEventListener<PlayerHitEvent>,
              public IEventListener<WallStateChangedEvent>,
              public IEventListener<PlayerMovedEvent>
{
private:
   Position position;
   Direction direction;
   Maze* maze = nullptr;
   VisualComponent visual;
public:

   Player(Position position, Direction direction);

   void onEvent(const PlayerHitEvent &event) override;
   void onEvent(const WallStateChangedEvent &event) override;
   void onEvent(const PlayerMovedEvent &event) override;
   void setMaze(Maze* m){maze=m;}
   
   void updateVisual(float deltaTime) { visual.update(deltaTime); }
   FloatPos getRenderPosition() const { return visual.getRenderPosition(); }

   Position getPosition() const{ return position; }
   Direction getDirection() const{ return direction;}
   bool isVisualComplete() const {return visual.getProgress()>=1.0f;}
};
