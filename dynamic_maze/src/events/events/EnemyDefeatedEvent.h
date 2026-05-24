#pragma once
#include "../../core/Position.h"

class IEnemy;

class EnemyDefeatedEvent{
public:
   IEnemy* enemy;
   Position position;
};


