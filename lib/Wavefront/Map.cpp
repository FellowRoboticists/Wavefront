#include <Arduino.h>
#include "Coordinate.h"
#include "MinValueDirection.h"
#include "IWavefront.h"
#include "Map.h"

#define PROPAGATE_ITERATIONS 50

Map::Map() : Map::Map(DEFAULT_X_SIZE, DEFAULT_Y_SIZE) {
}

Map::Map(int sizeX, int sizeY) : Map(sizeX, sizeY, DEFAULT_DIM_X, DEFAULT_DIM_Y) {
}

Map::Map(int sizeX, int sizeY, double dimX, double dimY) {
  mSizeX = sizeX;
  mSizeY = sizeY;
  mDimX = dimX;
  mDimY = dimY;

  buildMap(sizeX, sizeY);
}

int Map::getSizeX() {
  return mSizeX;
}

int Map::getSizeY() {
  return mSizeY;
}

void Map::placeValue(int x, int y, int value) {
  if (! coordinateInRange(x, y)) {
    return;
  }

  mMap[x][y] = value;
}

int Map::getValue(int x, int y) {
  int value = NOTHING;
  if (coordinateInRange(x, y)) {
    value = mMap[x][y];
  }
  return value;
}

void Map::minSurroundingNode(int x, int y, MinValueDirection &mvd) {
  mvd.setNodeValue(RESET_MIN);
  mvd.resetDirection();

  if (! coordinateInRange(x, y)) {
    return;
  }

  if (nodeLessThanMinimum(x + 1, y, mvd.getNodeValue())) {
    mvd.setNodeValue(mMap[x + 1][y]);
    mvd.setDirection(DOWN);
  }

  if (nodeLessThanMinimum(x - 1, y, mvd.getNodeValue())) {
    mvd.setNodeValue(mMap[x - 1][y]);
    mvd.setDirection(UP);
  }

  if (nodeLessThanMinimum(x, y + 1, mvd.getNodeValue())) {
    mvd.setNodeValue(mMap[x][y + 1]);
    mvd.setDirection(RIGHT);
  }

  if (nodeLessThanMinimum(x, y - 1, mvd.getNodeValue())) {
    mvd.setNodeValue(mMap[x][y - 1]);
    mvd.setDirection(LEFT);
  }
}

void Map::clear() {
  for (int x=0; x<mSizeX; x++) {
    for (int y=0; y<mSizeY; y++) {
      if (mMap[x][y] != ROBOT && mMap[x][y] != GOAL) {
        mMap[x][y] = NOTHING;
      }
    }
  }
}

void Map::unpropagate() {
  for (int x=0; x<mSizeX; x++) {
    for (int y=0; y<mSizeY; y++) {
      if (mMap[x][y] != ROBOT && mMap[x][y] != GOAL && mMap[x][y] != WALL) {
        mMap[x][y] = NOTHING;
      }
    }
  }
}

int Map::propagateWavefront(IWavefront *wavefront) {
  unpropagate();
  int v = 0;

  // Show the state of the map prior to propagation
  if (wavefront) {
    wavefront->wave(*this);
  }

  for (int i=0; i<PROPAGATE_ITERATIONS; i++) {
    for (int x=0; x<mSizeX; x++) {
      for (int y=0; y<mSizeY; y++) {
        if (mMap[x][y] == WALL || mMap[x][y] == GOAL) {
          continue;
        }

        MinValueDirection mvd(RESET_MIN, NOTHING);
        minSurroundingNode(x, y, mvd);
        if (mvd.getNodeValue() < RESET_MIN && mMap[x][y] == ROBOT) {
          if (wavefront) {
            wavefront->wave(*this);
          }
          return mvd.getDirection();
        } else if (mvd.getNodeValue() != RESET_MIN) {
          mMap[x][y] = mvd.getNodeValue() + 1;
        }
      }
    }
    // Call the callback if registered for each step of
    // the propagation
    if (wavefront) {
      wavefront->wave(*this);
    }
  }
}

Coordinate *Map::gridLocationFromCenterRadius(int x, int y, double angle, double radius) {
  // Determine the physical location of the X, Y location
  double physX = x * mDimX + (mDimX / 2.0);
  double physY = y * mDimY + (mDimY / 2.0);
  double radAngle = angle * PI / 180.0;
  double targetX = physX + radius * cos(radAngle);
  double targetY = physY + radius * sin(radAngle);

  // Hand back grid tuple
  return new Coordinate((int)round((targetX - mDimX / 2.0) / mDimX),
                        (int)round((targetY - mDimY / 2.0) / mDimY));
}

boolean Map::coordinateInRange(int x, int y) {
  return x >= 0 && x < mSizeX && y >= 0 && y < mSizeY;
}

boolean Map::nodeLessThanMinimum(int x, int y, int minimum) {
  return coordinateInRange(x, y) && mMap[x][y] != NOTHING && mMap[x][y] < minimum;
}

void Map::buildMap(int sizeX, int sizeY) {
  for (int x=0; x<mSizeX; x++) {
    for (int y=0; y<mSizeY; y++) {
      mMap[x][y] = NOTHING;
    }
  }
}
