#ifndef _Map_h_
#define _Map_h_

#define NOTHING 0
#define WALL 255
#define GOAL 1
#define ROBOT 254

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define RESET_MIN 250

#define DEFAULT_DIM_X 33.0
#define DEFAULT_DIM_Y 33.0

#define DEFAULT_X_SIZE 10
#define DEFAULT_Y_SIZE 10

class Coordinate;

class MinValueDirection;

class Map {

  public:

    Map();

    int getSizeX();

    int getSizeY();

    void placeValue(int x, int y, int value);

    int getValue(int x, int y);

    MinValueDirection *minSurroundingNode(int x, int y);

    void clear();

    void unpropagate();

    int propagateWavefront();

    Coordinate *gridLocationFromCenterRadius(int x, int y, double angle, double radius);

  private:

    Map(int sizeX, int sizeY);
    Map(int sizeX, int sizeY, double dimX, double dimY);
    boolean coordinateInRange(int x, int y);

    boolean nodeLessThanMinimum(int x, int y, int minimum);

    void buildMap(int sizeX, int sizeY);

    int mSizeX;
    int mSizeY;
    double mDimX;
    double mDimY;
    int mMap[DEFAULT_X_SIZE][DEFAULT_Y_SIZE];

};

#endif
