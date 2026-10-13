#pragma once
#include "../../../core/Position.h"
#include "../../../core/Cell.h"

class IHeuristic{
public:
   virtual float estimate(const Position& a, const Position& b) const = 0;
   virtual ~IHeuristic() = default;
};
