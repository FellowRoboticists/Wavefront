#ifndef _MinValueDirection_h_
#define _MinValueDirection_h_


class MinValueDirection {

  public:
    MinValueDirection(uint8_t value, uint8_t direction);

    boolean directionSet();

    void setNodeValue(uint8_t value);
    uint8_t getNodeValue();

    void setDirection(uint8_t direction);
    uint8_t getDirection();
    void resetDirection();

  private:

    uint8_t mNodeValue;
    uint8_t mDirection;
    uint8_t mNullDirection;
};

#endif
