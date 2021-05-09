#ifndef NEMASTEPPER_H
#define NEMASTEPPER_H

#define SHORT_DELAY 100

#include "Arduino.h"
#include "LimitSwitch.h"

class NemaStepper {
  public:
    NemaStepper();

    NemaStepper(unsigned int pinDir, unsigned int pinEn, unsigned int pinStep);

    void calibrateTo(LimitSwitch &limitSwitch);

    void disable();

    void enable();

    unsigned int getStepsPerRevolution() const;

    unsigned int getPulseDelay() const;

    unsigned int read() const;

    void setEnglandMode(bool englandMode);

    void setStepsPerRevolution(unsigned int stepsPerRevolution);

    void setPulseDelay(unsigned int pulseDelay);

    void setZero();

    void write(int degrees);
    
  private:
    bool englandMode                = false;
  
    unsigned int stepsPerRevolution = 200;
    unsigned int pulseDelay         = 700;

    unsigned int pinDir             =   0;
    unsigned int pinEn              =   0;
    unsigned int pinStep            =   0;

    unsigned int steps              =   0;
};

#endif
