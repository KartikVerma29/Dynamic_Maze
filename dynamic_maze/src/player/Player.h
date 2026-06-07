#pragma once
#include "../events/IEventListener.h"
#include "../events/events/PlayerMovedEvent.h"
#include "../events/events/PlayerHitEvent.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../core/Position.h"
#include "../core/Direction.h"


class Player: public IEventListener<PlayerHitEvent>,
              public IEventListener<WallStateChangedEvent>,
              public IEventListener<PlayerMovedEvent>
{
private:
   Position position;
   Direction direction;
public:

   Player(Position position, Direction direction);

   void onEvent(const PlayerHitEvent &event) override;
   void onEvent(const WallStateChangedEvent &event) override;
   void onEvent(const PlayerMovedEvent &event) override;
   
   Position getPosition() const{ return position; }
   Direction getDirection() const{ return direction;}
};
