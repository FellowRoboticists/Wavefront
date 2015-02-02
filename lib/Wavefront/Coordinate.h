#ifndef _Coordinate_h_
#define _Coordinate_h_

class Coordinate {
  
  public:

    Coordinate();
    Coordinate(int x, int y);
    
    boolean onTheGrid(int maxX, int maxY);
    void setCoordinates(int x, int y);
    int getX();
    int getY();

  private:

    int mX;
    int mY;

};

#endif
