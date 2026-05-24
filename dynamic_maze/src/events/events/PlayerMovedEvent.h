#pragma once
#include "../../core/Direction.h"
#include "../../core/Position.h"

class PlayerMovedEvent{
public:
   Direction direction;
   Position newPosition;
};

