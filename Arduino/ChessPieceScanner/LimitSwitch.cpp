#include "LimitSwitch.h"

LimitSwitch::LimitSwitch() : pinSwitch(65535) { }

LimitSwitch::LimitSwitch(unsigned int pinSwitch) : pinSwitch(pinSwitch) {
  pinMode(this->pinSwitch, INPUT);
}

bool LimitSwitch::isPressed() const {
  return digitalRead(this->pinSwitch) == HIGH;
}

bool LimitSwitch::isReleased() const {
  return !this->isPressed();
}
