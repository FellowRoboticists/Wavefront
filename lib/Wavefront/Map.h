#ifndef _Map_h_
#define _Map_h_

#define NOTHING (uint8_t)0
#define WALL (uint8_t)255
#define GOAL (uint8_t)1
#define ROBOT (uint8_t)254

#define UP (uint8_t)1
#define RIGHT (uint8_t)2
#define DOWN (uint8_t)3
#define LEFT (uint8_t)4

#define RESET_MIN (uint8_t)250

#define DEFAULT_DIM_X 33.0
#define DEFAULT_DIM_Y 33.0

#define DEFAULT_X_SIZE (uint8_t)10
#define DEFAULT_Y_SIZE (uint8_t)10

class Coordinate;

class MinValueDirection;

class IWavefront;

class Map {

  public:

    Map();

    uint8_t getSizeX();

    uint8_t getSizeY();

    void placeValue(uint8_t x, uint8_t y, uint8_t value);

    uint8_t getValue(uint8_t x, uint8_t y);

    void minSurroundingNode(uint8_t x, uint8_t y, MinValueDirection& mvd);

    void clear();

    void unpropagate();

    uint8_t propagateWavefront(IWavefront *wavefront);

    void gridLocationFromCenterRadius(uint8_t x, uint8_t y, double angle, double radius, Coordinate& coordinat);
    boolean coordinateInRange(uint8_t x, uint8_t y);
    boolean nodeLessThanMinimum(uint8_t x, uint8_t y, uint8_t minimum);

  private:

    Map(uint8_t sizeX, uint8_t sizeY);
    Map(uint8_t sizeX, uint8_t sizeY, double dimX, double dimY);


    void buildMap(uint8_t sizeX, uint8_t sizeY);

    uint8_t mSizeX;
    uint8_t mSizeY;
    double mDimX;
    double mDimY;
    uint8_t mMap[DEFAULT_X_SIZE][DEFAULT_Y_SIZE];

};

#endif
