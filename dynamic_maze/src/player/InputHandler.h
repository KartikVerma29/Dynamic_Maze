#pragma once
#include "../core/Direction.h"
#include "../events/EventManager.h"
#include <optional>
#include "../events/events/PlayerMovedEvent.h"
#include <raylib.h>

class InputHandler{
private:
   EventManager& eventManager;
   std::optional<Direction> readKeyboard();
public:
   InputHandler(EventManager& eventManager) : eventManager(eventManager){}
   void update();
};
