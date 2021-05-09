#ifndef LIMITSWITCH_H
#define LIMITSWITCH_H

#include "Arduino.h"

class LimitSwitch {
  public:
    LimitSwitch();

    LimitSwitch(unsigned int pinSwitch);

    bool isPressed() const;

    bool isReleased() const;
  
  private:
    unsigned int pinSwitch = 0;
};

#endif
