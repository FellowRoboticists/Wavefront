#include <Arduino.h>
#include "Coordinate.h"

Coordinate::Coordinate() : Coordinate::Coordinate(0, 0) {
}

Coordinate::Coordinate(uint8_t x, uint8_t y) {
  mX = x;
  mY = y;
}

void Coordinate::setCoordinates(uint8_t x, uint8_t y) {
  mX = x;
  mY = y;
}

boolean Coordinate::onTheGrid(uint8_t maxX, uint8_t maxY) {
  return mX >= 0 && mX < maxX && mY >= 0 && mY < maxY;
}

uint8_t Coordinate::getX() {
  return mX;
}

uint8_t Coordinate::getY() {
  return mY;
}
