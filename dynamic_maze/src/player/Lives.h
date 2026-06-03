#pragma once

class Lives{
private:
   int count;
   int maxlife;

public:
   Lives(int count, int maxlife): count(count), maxlife(maxlife) {}

   void decrement(){
      if(count>0) count--;
   };
   bool isGameOver() const{ return count==0 ;}
   int getCount() const{ return count;}
};
