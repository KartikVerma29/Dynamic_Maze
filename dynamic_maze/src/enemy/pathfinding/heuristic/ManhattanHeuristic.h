#pragma once
#include "IHeuristic.h"
#include <cmath>

class Manhattan: public IHeuristic{
public:
   float estimate(const Position&a, const Position& b) const override{
      return std::abs(a.getX()-b.getX()) + std::abs(a.getY()-b.getY());
   }
};
