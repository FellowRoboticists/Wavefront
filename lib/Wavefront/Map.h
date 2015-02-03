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

    int getSizeX();

    int getSizeY();

    void placeValue(int x, int y, int value);

    int getValue(int x, int y);

    void minSurroundingNode(int x, int y, MinValueDirection& mvd);

    void clear();

    void unpropagate();

    int propagateWavefront(IWavefront *wavefront);

    void gridLocationFromCenterRadius(int x, int y, double angle, double radius, Coordinate& coordinat);
    boolean coordinateInRange(int x, int y);
    boolean nodeLessThanMinimum(int x, int y, int minimum);

  private:

    Map(int sizeX, int sizeY);
    Map(int sizeX, int sizeY, double dimX, double dimY);


    void buildMap(int sizeX, int sizeY);

    int mSizeX;
    int mSizeY;
    double mDimX;
    double mDimY;
    int mMap[DEFAULT_X_SIZE][DEFAULT_Y_SIZE];

};

#endif
