#include <Arduino.h>
#include "Coordinate.h"

Coordinate::Coordinate() : Coordinate::Coordinate(0, 0) {
}

Coordinate::Coordinate(int x, int y) {
  mX = x;
  mY = y;
}

void Coordinate::setCoordinates(int x, int y) {
  mX = x;
  mY = y;
}

boolean Coordinate::onTheGrid(int maxX, int maxY) {
  return mX >= 0 && mX < maxX && mY >= 0 && mY < maxY;
}

int Coordinate::getX() {
  return mX;
}

int Coordinate::getY() {
  return mY;
}
