#include "InputHandler.h"
#include "../events/events/PlayerMovedEvent.h"
#include <optional>


void InputHandler::update(){
   auto dir = readKeyboard();
   if(dir.has_value()){
      eventManager.publish(PlayerMovedEvent{dir.value(), Position(0,0)});
   }
}

std::optional<Direction> InputHandler::readKeyboard(){
   return std::nullopt;
}
