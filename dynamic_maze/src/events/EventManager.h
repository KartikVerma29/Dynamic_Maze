#pragma once
#include "IEventListener.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <algorithm>


class EventManager{
private:
   std::unordered_map<std::type_index, std::vector<void*>> listeners;
public:
   template<typename T>
   void subscribe(IEventListener<T>& listener){
      listeners[typeid(T)].push_back(&listener);
   }

   template<typename T>
   void unsubcribe(IEventListener<T>& listener){
      auto& vec = listeners[typeid(T)];
      vec.erase(std::remove(vec.begin(), vec.end(), &listener), vec.end());
   }

  template<typename T>

   void publish(const T& event){
      auto it=listeners.find(typeid(T));
      if(it!=listeners.end()){
         std::vector<void*> safeCopy = it->second;
         for(void* ptr: safeCopy){
            static_cast<IEventListener<T>*>(ptr)->onEvent(event);
         }
      }
   }

};
