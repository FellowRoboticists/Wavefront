#ifndef _Coordinate_h_
#define _Coordinate_h_

class Coordinate {
  
  public:

    Coordinate();
    Coordinate(uint8_t x, uint8_t y);
    
    boolean onTheGrid(uint8_t maxX, uint8_t maxY);
    void setCoordinates(uint8_t x, uint8_t y);
    uint8_t getX();
    uint8_t getY();

  private:

    uint8_t mX;
    uint8_t mY;

};

#endif
