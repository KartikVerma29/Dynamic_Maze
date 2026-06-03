#pragma once
#include "IEventListener.h"
template<typename T>

class IEventPublisher{
public:
   virtual void subscribe(IEventListener<T>& listener) = 0;
   virtual void unsubscribe(IEventListener<T>& listener) = 0;
   virtual void publish(const T& event) = 0;

   virtual ~IEventPublisher() = default;
};
