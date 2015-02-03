#include <Arduino.h>
#include "MinValueDirection.h"

MinValueDirection::MinValueDirection(uint8_t value, uint8_t direction) {
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

void MinValueDirection::setNodeValue(uint8_t value) {
  mNodeValue = value;
}

uint8_t MinValueDirection::getNodeValue() {
  return mNodeValue;
}

void MinValueDirection::setDirection(uint8_t direction) {
  mDirection = direction;
}

uint8_t MinValueDirection::getDirection() {
  return mDirection;
}

