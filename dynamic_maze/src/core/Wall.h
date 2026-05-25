#pragma once
#include <utility>
class Wall{
private:
   int row1, col1;
   int row2, col2;
   bool isOpen;
public:
   Wall(int row1, int col1, int row2, int col2, bool isOpen=false) 
      : row1(row1), col1(col1), row2(row2), col2(col2), isOpen(isOpen){}

   void open(){isOpen=true;}
   void close(){isOpen=false;}
   bool getIsOpen() const{return isOpen;}
   bool connects(int row, int col) const{
      return (row==row1 && col==col1) || (row==row2 && col==col2);
   }
   std::pair<std::pair<int,int>,std::pair<int,int>> getCells() const{
      return {{row1, col1},{row2, col2}};
   }
};
