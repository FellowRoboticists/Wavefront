#ifndef _Coordinate_h_
#define _Coordinate_h_

class Coordinate {
  
  public:

    Coordinate(int x, int y);
    
    boolean onTheGrid(int maxX, int maxY);

  private:

    int mX;
    int mY;

};

#endif
