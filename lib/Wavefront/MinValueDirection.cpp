#include <Arduino.h>
#include "MinValueDirection.h"

MinValueDirection::MinValueDirection(int value, int direction) {
  mNodeValue = value;
  mDirection = direction;
  mNullDirection = direction;
}

boolean MinValueDirection::directionSet() {
  return mDirection != mNullDirection;
}

void MinValueDirection::resetDirection() {
  mDirection = 0;
  mNullDirection = 0;
}

void MinValueDirection::setNodeValue(int value) {
  mNodeValue = value;
}

int MinValueDirection::getNodeValue() {
  return mNodeValue;
}

void MinValueDirection::setDirection(int direction) {
  mDirection = direction;
}

int MinValueDirection::getDirection() {
  return mDirection;
}

