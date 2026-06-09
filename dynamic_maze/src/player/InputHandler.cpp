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
    if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) return Direction(DirectionType::UP);
    if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) return Direction(DirectionType::DOWN);
    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) return Direction(DirectionType::LEFT);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return Direction(DirectionType::RIGHT);
    return std::nullopt;
}
