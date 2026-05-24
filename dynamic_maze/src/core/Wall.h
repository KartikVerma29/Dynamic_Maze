#pragma once
#include <utility>
class Wall{
private:
   int row1, col1;
   int row2, col2;
   bool isOpen;
public:
   Wall(int row1, int col1, int row2, int col2);

   void open() ;
   void close() ;
   bool getIsOpen() const;
   bool connects(int row, int col) const;
   std::pair<std::pair<int,int>,std::pair<int,int>> getCells() const;
};
