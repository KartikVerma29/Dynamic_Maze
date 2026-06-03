#pragma once
#include "../../core/Position.h"

class IEnemy;

class PlayerHitEvent{
public:
   Position hitPoint;
   IEnemy* source;
};
