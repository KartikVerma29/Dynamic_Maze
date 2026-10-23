#pragma once
#include "IEventListener.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ostream>

struct Stats{long subcribes=0, unsubcribes=0, publishes=0, listenerCalls=0;};


class EventManager{
private:
   std::unordered_map<std::type_index, std::vector<void*>> listeners;
   std::unordered_map<std::type_index,Stats> stats;
public:
   template<typename T>
   void subscribe(IEventListener<T>& listener){
      listeners[typeid(T)].push_back(&listener);
      stats[typeid(T)].subcribes++;
   }

   template<typename T>
   void unsubscribe(IEventListener<T>& listener){
      auto& vec = listeners[typeid(T)];
      vec.erase(std::remove(vec.begin(), vec.end(), &listener), vec.end());
      stats[typeid(T)].unsubcribes++;
   }

  template<typename T>

   void publish(const T& event){
      auto it=listeners.find(typeid(T));
      stats[typeid(T)].publishes++;
      if(it!=listeners.end()){
         std::vector<void*> safeCopy = it->second;
         for(void* ptr: safeCopy){
            static_cast<IEventListener<T>*>(ptr)->onEvent(event);
            stats[typeid(T)].listenerCalls++;
         }
      }
   }

  void printStats(){
     for(auto& [t,s]:stats){
        std::printf("\n%s sub=%ld unsub=%ld pub=%ld calls=%ld\n",
              t.name(), s.subcribes, s.unsubcribes, s.publishes, s.listenerCalls);
     }
  }

  void resetStats(){ stats.clear();}

};
