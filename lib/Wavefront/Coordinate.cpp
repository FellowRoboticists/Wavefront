#include <Arduino.h>
#include "Coordinate.h"

Coordinate::Coordinate(int x, int y) {
  mX = x;
  mY = y;
}

boolean Coordinate::onTheGrid(int maxX, int maxY) {
  return mX >= 0 && mX < maxX && mY >= 0 && mY < maxY;
}
