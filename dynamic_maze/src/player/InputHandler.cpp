#include "InputHandler.h"
#include "../events/events/PlayerMovedEvent.h"
#include <optional>
#include <raylib.h>


void InputHandler::update(){
   auto dir = readKeyboard();
   if(dir.has_value()){
      eventManager.publish(PlayerMovedEvent{dir.value()});
   }
}

std::optional<Direction> InputHandler::readKeyboard() {
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) return Direction(DirectionType::UP);
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) return Direction(DirectionType::DOWN);
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) return Direction(DirectionType::LEFT);
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) return Direction(DirectionType::RIGHT);
    return std::nullopt;
}
