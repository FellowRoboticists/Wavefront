#ifndef _MinValueDirection_h_
#define _MinValueDirection_h_


class MinValueDirection {

  public:
    MinValueDirection(int value, int direction);

    boolean directionSet();

    void setNodeValue(int value);
    int getNodeValue();

    void setDirection(int direction);
    int getDirection();
    void resetDirection();

  private:

    int mNodeValue;
    int mDirection;
    int mNullDirection;
};

#endif
