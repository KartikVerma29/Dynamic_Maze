#pragma once

template<typename T >

class IEventListener{
public:
   virtual void onEvent(const T& event) = 0;
   virtual ~IEventListener() = default;
};
