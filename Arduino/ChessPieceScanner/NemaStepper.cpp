#include "NemaStepper.h"

NemaStepper::NemaStepper() : pinDir(65535), pinEn(65535), pinStep(65535) { }

NemaStepper::NemaStepper(unsigned int pinDir, unsigned int pinEn, unsigned int pinStep) : pinDir(pinDir), pinEn(pinEn), pinStep(pinStep) {
  pinMode(this->pinDir, OUTPUT);
  pinMode(this->pinEn, OUTPUT);
  pinMode(this->pinStep, OUTPUT);

  digitalWrite(this->pinDir, LOW);
  delayMicroseconds(SHORT_DELAY);
  digitalWrite(this->pinEn, HIGH);
  delayMicroseconds(SHORT_DELAY);
  digitalWrite(this->pinStep, LOW);
  delayMicroseconds(SHORT_DELAY);
}

void NemaStepper::calibrateTo(LimitSwitch &limitSwitch) {  
  this->write(25);

  delay(SHORT_DELAY);

  if (this->englandMode)
    digitalWrite(this->pinDir, HIGH);
  else
    digitalWrite(this->pinDir, LOW);
  delayMicroseconds(SHORT_DELAY);

  while (limitSwitch.isReleased()) {
    digitalWrite(this->pinStep, HIGH);
    delayMicroseconds(this->pulseDelay);
    digitalWrite(this->pinStep, LOW);
    delayMicroseconds(this->pulseDelay);
  }

  this->setZero();
}

void NemaStepper::disable() {
  digitalWrite(this->pinEn, HIGH);
  delayMicroseconds(SHORT_DELAY);
}

void NemaStepper::enable() {
  digitalWrite(this->pinEn, LOW);
  delayMicroseconds(SHORT_DELAY);
}

unsigned int NemaStepper::getStepsPerRevolution() const {
  return this->stepsPerRevolution;
}

unsigned int NemaStepper::getPulseDelay() const {
  return this->pulseDelay;
}

unsigned int NemaStepper::read() const {
  return (unsigned long)(this->steps) * (unsigned long)(360) / (unsigned long)(this->stepsPerRevolution);
}

void NemaStepper::setEnglandMode(bool englandMode) {
  this->englandMode = englandMode;
}

void NemaStepper::setStepsPerRevolution(unsigned int stepsPerRevolution) {
  this->stepsPerRevolution = stepsPerRevolution;
}

void NemaStepper::setPulseDelay(unsigned int pulseDelay) {
  this->pulseDelay = pulseDelay;
}

void NemaStepper::setZero() {
  this->steps = 0;
}

void NemaStepper::write(int degrees) {
  long steps, realSteps, from, to, i;
  this->enable();

  steps = long(degrees) * long(this->stepsPerRevolution) / long(360);
  
  realSteps = steps;
  while (realSteps < 0)
    realSteps = long(realSteps) + long(this->stepsPerRevolution);

  if (realSteps != this->steps) {
    if (steps > this->steps) {
      if (this->englandMode)
        digitalWrite(this->pinDir, LOW);
      else
        digitalWrite(this->pinDir, HIGH);

      from = this->steps;
      to = steps;
    }
    else {
      if (this->englandMode)
        digitalWrite(this->pinDir, HIGH);
      else
        digitalWrite(this->pinDir, LOW);

      from = steps;
      to = this->steps;
    }

    delayMicroseconds(SHORT_DELAY);

    for (i = from; i < to; i++) {
      digitalWrite(this->pinStep, HIGH);
      delayMicroseconds(this->pulseDelay);
      digitalWrite(this->pinStep, LOW);
      delayMicroseconds(this->pulseDelay);
    }

    this->steps = realSteps % this->stepsPerRevolution;
  }
}
