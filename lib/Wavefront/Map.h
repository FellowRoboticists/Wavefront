#ifndef _Map_h_
#define _Map_h_

/**
 * These manifest constants define special values to be placed on
 * the map:
 *
 * NOTHING - Placed on a grid cell that does not have a WALL, GOAL
 *           or ROBOT on it and where the cell does not have a 
 *           propagation value.
 * WALL    - Placed on a grid cell that has an obstacle on it.
 * GOAL    - Placed on a grid cell to indicate the goal of a 
 *           robot's movement.
 * ROBOT   - Placed on a grid cell at the robot's current location.
 */
#define NOTHING (uint8_t)0
#define WALL (uint8_t)255
#define GOAL (uint8_t)1
#define ROBOT (uint8_t)254

/**
 * These manifest constants define navigation directionality. The
 * direction values are based on a grid laid out as follows:
 *
 *      <---       Increasing Y values == RIGHT ---->
 *    ^
 *    |
 *    |
 *
 *    I
 *    n
 *    c
 *    r
 *    e
 *    a
 *    s
 *    i
 *    n
 *    g
 *
 *    X
 *
 *    v
 *    a
 *    l
 *    u
 *    e
 *    s
 *
 *    =
 *    =
 *
 *    D
 *    O
 *    W
 *    N
 *
 *    |
 *    |
 *    v
 *
 *  There is no conception of real-world directionality, hence
 *  no North, South, East or West. It is completely arbitrary and
 *  up to the user to determine.
 */
#define UP (uint8_t)1
#define RIGHT (uint8_t)2
#define DOWN (uint8_t)3
#define LEFT (uint8_t)4

/**
 * This manifest constant is a special value to help determine how
 * to unpropagate the wavefront for a map.
 */
#define RESET_MIN (uint8_t)250

/**
 * These manifest constants define the real-world size of each grid
 * cell. By convention, the units are in centimeters, but there is 
 * nothing in the library that depends on the units of these values.
 */
#define DEFAULT_DIM_X 33.0
#define DEFAULT_DIM_Y 33.0

/**
 * These manifest constants define the number of grid cells in the
 * map. These are set initially to a 10x10 grid. Bear in mind that
 * larger dimensions will use more memory. Also remember that the
 * library has been written to assume that each grid cell can be
 * reference by an 8-bit unsigned integer, so you can't have more
 * than 255 rows or columns. (The dimension 0xff is a special
 * value indicating an out-of-range dimension).
 */
#define DEFAULT_X_SIZE (uint8_t)10
#define DEFAULT_Y_SIZE (uint8_t)10

class Coordinate;

class MinValueDirection;

class IWavefront;

/**
 * An abstraction of a map, allowing different values to be placed at
 * specific locations on the map. The default size of the grid is
 * DEFAULT_X_SIZE x DEFAULT_X_SIZE. The size of each real-world grid
 * square is DEFAULT_DIM_X x DEFAULT_DIM_Y.
 */
class Map {

  public:

    /**
     * Constructs a new Map with the default dimensions and all
     * grid-cells set to NOTHING.
     */
    Map();

    /**
     * Gets the number of X grid cells in the map.
     */
    uint8_t getSizeX();

    /**
     * Gets the number of Y grid cells in the map.
     */
    uint8_t getSizeY();

    /**
     * Sets the value of the specified grid cell. This method is typically
     * used to place the robot, walls or the goal on the map. Other values
     * are usually defined by wave propagation.
     */
    void placeValue(uint8_t x, uint8_t y, uint8_t value);

    /**
     * Gets the value of the specified grid cell.
     */
    uint8_t getValue(uint8_t x, uint8_t y);

    /**
     * Looks at the immediately surrounding grid cells and populates the
     * MinValueDirection object with the value of the grid cell that is the 
     * smallest value and the direction of the minimum cell from the 
     * reference cell. 
     *
     * To be used, the surrounding grid cell value must be
     * greater than NOTHING but less than RESET_MIN. This is the core of 
     * the wave propagation algorithm; this method is called against 
     * each grid cell in the map and determines the new value for the 
     * cell.
     */
    void minSurroundingNode(uint8_t x, uint8_t y, MinValueDirection& mvd);

    /**
     * Resets all grid cells to NOTHING except those marked with ROBOT
     * or GOAL.
     *
     * You would typically call this when your robot has moved to a new
     * grid cell and you want to re-scan for obstacles. After clearing,
     * you would then re-propagate.
     */
    void clear();

    /**
     * Resets all grid cells to NOTHING except those marked with ROBOT,
     * GOAL or WALL.
     */
    void unpropagate();

    /**
     * Initiates the wave-front propagation algorithm on the map. You
     * would normally call this after calling Map::clear() and placing
     * any obstacles.
     *
     * The wave-front propagation algorithm will process until it finds
     * a path between the ROBOT and the GOAL. If a path is found, this 
     * method returns a direction in which the robot can initiate it's 
     * trip to the goal. If no path is found, NOTHING is returned. In 
     * this case, you would typically call Map::clear(), rescan for
     * obstacles (in case one of the obstacles was moved) and 
     * re-propagate.
     *
     * If you pass a pointer to an implementation of the IWavefront
     * interface, your implementation will be called once for each
     * iteration of the wave-front propagation algorithm. This is
     * handy to visualize how the algorithm finds its path. If you
     * don't wish to pass an implementation, simply pass NULL as the
     * argument.
     */
    uint8_t propagateWavefront(IWavefront *wavefront);

    /**
     * Populates the reference to the coordinate with the map grid
     * coordinate that is indicated by placing the center of circle
     * at a specified grid location (e.g. the current grid location
     * of the ROBOT, using an angle (radians) and out a distance of
     * radius. Think of this has having a sensor that can look out
     * at a particular angle and can sense the distance to an 
     * obstacle.
     *
     * The radius value is conventionally in centimeters, however as long
     * as its units match that of DEFAULT_DIM_X and DEFAULT_DIM_Y you
     * should be fine with any other units.
     *
     * When returned, you may place a value at the specified coordinate
     * that is indicative of what's been sensed.
     */
    void gridLocationFromCenterRadius(uint8_t x, 
                                      uint8_t y, 
                                      double angle, 
                                      double radius, 
                                      Coordinate& coordinate);

    /**
     * Returns true if the specified coordinate values exist on the 
     * map.
     */
    boolean coordinateInRange(uint8_t x, uint8_t y);

    /**
     * Returns true if the current value of the grid cell is not NOTHING
     * and is less than the minimum specified value. This is heavily 
     * used by the wave-front propagation algorithm.
     */
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
