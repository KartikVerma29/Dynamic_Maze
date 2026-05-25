#pragma once
//<<event>>
class Wall;
class WallAddedEvent{
public:
   int row1, col1;
   int row2, col2;
   Wall* wall;
};
