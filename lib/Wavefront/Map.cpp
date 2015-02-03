#include <Arduino.h>
#include "Coordinate.h"
#include "MinValueDirection.h"
#include "IWavefront.h"
#include "Map.h"

#define PROPAGATE_ITERATIONS 50

Map::Map() : Map::Map(DEFAULT_X_SIZE, DEFAULT_Y_SIZE) {
}

Map::Map(uint8_t sizeX, uint8_t sizeY) : Map(sizeX, sizeY, DEFAULT_DIM_X, DEFAULT_DIM_Y) {
}

Map::Map(uint8_t sizeX, uint8_t sizeY, double dimX, double dimY) {
  mSizeX = sizeX;
  mSizeY = sizeY;
  mDimX = dimX;
  mDimY = dimY;

  buildMap(sizeX, sizeY);
}

uint8_t Map::getSizeX() {
  return mSizeX;
}

uint8_t Map::getSizeY() {
  return mSizeY;
}

void Map::placeValue(uint8_t x, uint8_t y, uint8_t value) {
  if (! coordinateInRange(x, y)) {
    return;
  }

  mMap[x][y] = value;
}

uint8_t Map::getValue(uint8_t x, uint8_t y) {
  uint8_t value = NOTHING;
  if (coordinateInRange(x, y)) {
    value = mMap[x][y];
  }
  return value;
}

void Map::minSurroundingNode(uint8_t x, uint8_t y, MinValueDirection &mvd) {
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
  for (uint8_t x=0; x<mSizeX; x++) {
    for (uint8_t y=0; y<mSizeY; y++) {
      if (mMap[x][y] != ROBOT && mMap[x][y] != GOAL) {
        mMap[x][y] = NOTHING;
      }
    }
  }
}

void Map::unpropagate() {
  for (uint8_t x=0; x<mSizeX; x++) {
    for (uint8_t y=0; y<mSizeY; y++) {
      if (mMap[x][y] != ROBOT && mMap[x][y] != GOAL && mMap[x][y] != WALL) {
        mMap[x][y] = NOTHING;
      }
    }
  }
}

uint8_t Map::propagateWavefront(IWavefront *wavefront) {
  unpropagate();

  // Show the state of the map prior to propagation
  if (wavefront) {
    wavefront->wave(*this);
  }

  for (uint8_t i=0; i<PROPAGATE_ITERATIONS; i++) {
    for (uint8_t x=0; x<mSizeX; x++) {
      for (uint8_t y=0; y<mSizeY; y++) {
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

void Map::gridLocationFromCenterRadius(uint8_t x, uint8_t y, double angle, double radius, Coordinate& coordinate) {
  // Determine the physical location of the X, Y location
  double physX = x * mDimX + (mDimX / 2.0);
  double physY = y * mDimY + (mDimY / 2.0);
  double radAngle = angle * PI / 180.0;
  double targetX = physX + radius * cos(radAngle);
  double targetY = physY + radius * sin(radAngle);

  int newX = round((targetX - mDimX / 2.0) / mDimX);
  int newY = round((targetY - mDimY / 2.0) / mDimY);

  // Hand back grid tuple
  coordinate.setCoordinates((newX >= 0) ? (uint8_t)newX : 0xff,
                            (newY >= 0) ? (uint8_t)newY : 0xff);
}

boolean Map::coordinateInRange(uint8_t x, uint8_t y) {
  return x >= 0 && x < mSizeX && y >= 0 && y < mSizeY;
}

boolean Map::nodeLessThanMinimum(uint8_t x, uint8_t y, uint8_t minimum) {
  return coordinateInRange(x, y) && mMap[x][y] != NOTHING && mMap[x][y] < minimum;
}

void Map::buildMap(uint8_t sizeX, uint8_t sizeY) {
  for (uint8_t x=0; x<mSizeX; x++) {
    for (uint8_t y=0; y<mSizeY; y++) {
      mMap[x][y] = NOTHING;
    }
  }
}
