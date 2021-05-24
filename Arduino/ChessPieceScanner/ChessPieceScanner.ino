/*
   Arduino UNO

           +------------------------------------------------+
   RX 0 --+|                                                |+-- A5
           |                                                |
   TX 1 --+|                                                |+-- A4
           |                                                |
      2 --+|                                                |+-- A3
           |                                                |
      3 --+|                                                |+-- A2
           |                                                |
      3 --+|                                                |+-- A1
           |                                                |
      4 --+|                                                |+-- A0
           |                                                |
      5 --+|                                                |
           |                                                |
      6 --+|                                                |+-- VIN
           |                                                |
      7 --+|                                                |+-- GND
           |                                                |
           |                                                |+-- GND
           |                                                |
      8 --+|                                                |+-- +5v
           |                                                |
      9 --+|                                                |+-- +3v
           |                                                |
     10 --+|                                                |+-- RST
           |                                                |
     11 --+|                                                |+-- IOREF
           |                                                |
     12 --+|                                                |
           |                                                |
     13 --+|                                                |
           |                                                |
    GND --+|                                                |
           |                                                |
   AREF --+|                                                |
           +------------------------------------------------+
*/

#include "Configuration.h"

/*
   Motor angular camara y rotacion de la base

           +------------+
           |            |+-- B2
           |            |+-- A2
           |     ()     |
           |            |+-- A1
           |            |+-- B1
           +------------+

           +------------+
   +12v --+|            |
           |            |
    GND --+|            |
           |            |
     B2 --+|            |
           |            |
     A2 --+|            |
           |            |
     A1 --+|            |+-+ RESET
           |            |  |
     B1 --+|            |+-+ SLEEP
           |            |
    +5v --+|            |+-- STEP
           |            |
   GNDL --+|            |+-- DIR
           +------------+
*/

#include "NemaStepper.h"
NemaStepper stepperZAxis;
NemaStepper stepperYAxis;

/*
   Finales de carrera


             _____/\
           +/-O---------+
           |            |
           |            |
           +-+----+---+-+
             |    |   |
            A0   +5v GND
*/

#include "LimitSwitch.h"
LimitSwitch stepperZAxisLimitswitch;
LimitSwitch stepperYAxisLimitswitch;

/*
   Servo

           ---+---
           +--+---------+
           |            |+-- DATA 5
           |            |+-- +5v
           |            |+-- GND
           +------------+


*/
#include <Servo.h>
Servo servoYAxis;

String usbSerialCommand;

#include "Command.h"
#define COMMANDS 9

unsigned int c;

Command CMD_INFORMATION_AUTHOR;

Command CMD_CALIBRATION_SETZERO;
Command CMD_CALIBRATION_CALIBRATE;

Command CMD_FUNCTION_LIBERATE;
Command CMD_FUNCTION_MOVETO;

Command CMD_QUERY_GETROTATION;
Command CMD_QUERY_GETLIMITSWITCHSTATUS;

Command CMD_CONFIGURATION_DELAY;
Command CMD_CONFIGURATION_STEPSPERREV;

Command *availableCommands[COMMANDS] = {
  &CMD_INFORMATION_AUTHOR,
  
  &CMD_CALIBRATION_SETZERO,
  &CMD_CALIBRATION_CALIBRATE,

  &CMD_FUNCTION_LIBERATE,
  &CMD_FUNCTION_MOVETO,

  &CMD_QUERY_GETROTATION,
  &CMD_QUERY_GETLIMITSWITCHSTATUS,

  &CMD_CONFIGURATION_DELAY,
  &CMD_CONFIGURATION_STEPSPERREV
};

void setup() {
  Serial.begin(USB_SERIAL_SPEED);

  stepperZAxis = NemaStepper(STEPPER_Z_AXIS_PIN_DIR,
                             STEPPER_Z_AXIS_PIN_EN,
                             STEPPER_Z_AXIS_PIN_STEP);

  stepperZAxis.setEnglandMode(STEPPER_Z_AXIS_ENGLAND_MODE);

  stepperZAxis.setStepsPerRevolution(STEPPER_Z_AXIS_RESOLUTION *
                                     STEPPER_Z_AXIS_HARDWARE_REDUCTION *
                                     STEPPER_Z_AXIS_PHYSICAL_REDUCTION);

  stepperZAxisLimitswitch = LimitSwitch(STEPPER_Z_AXIS_PIN_LIMITSWITCH);

  stepperYAxis = NemaStepper(STEPPER_Y_AXIS_PIN_DIR,
                             STEPPER_Y_AXIS_PIN_EN,
                             STEPPER_Y_AXIS_PIN_STEP);

  stepperYAxis.setEnglandMode(STEPPER_Y_AXIS_ENGLAND_MODE);

  stepperYAxis.setStepsPerRevolution(STEPPER_Y_AXIS_RESOLUTION *
                                     STEPPER_Y_AXIS_HARDWARE_REDUCTION *
                                     STEPPER_Y_AXIS_PHYSICAL_REDUCTION);

  stepperYAxisLimitswitch = LimitSwitch(STEPPER_Y_AXIS_PIN_LIMITSWITCH);

  pinMode(LED_BUILTIN, OUTPUT);

  CMD_INFORMATION_AUTHOR         = Command(0, 0, NULL);

  CMD_CALIBRATION_SETZERO        = Command(100, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'Y')
      stepperYAxis.setZero();
    else if (arguments[0].letter == 'Z')
      stepperZAxis.setZero();
  });

  CMD_CALIBRATION_CALIBRATE      = Command(101, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'Y')
      stepperYAxis.calibrateTo(stepperYAxisLimitswitch);
    else if (arguments[0].letter == 'Z')
      stepperZAxis.calibrateTo(stepperZAxisLimitswitch);
  });

  CMD_FUNCTION_LIBERATE          = Command(200, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'J')
      servoYAxis.detach();
    else if (arguments[0].letter == 'Y')
      stepperYAxis.disable();
    else if (arguments[0].letter == 'Z')
      stepperZAxis.disable();
  });

  CMD_FUNCTION_MOVETO            = Command(201, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'J') {
      if (arguments[0].number >= 0 && arguments[0].number <= 120) {
        servoYAxis.attach(SERVO_Y_AXIS);
        servoYAxis.write(arguments[0].number);
        delay(500);
      }
    }
    else if (arguments[0].letter == 'Y') {
      if (arguments[0].number >= 0 && arguments[0].number <= 90)
        stepperYAxis.write(arguments[0].number);
    }
    else if (arguments[0].letter == 'Z')
      stepperZAxis.write(arguments[0].number);
  });

  CMD_QUERY_GETROTATION          = Command(300, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'J')
      CMD_QUERY_GETROTATION.echoReturn(servoYAxis.read());
    else if (arguments[0].letter == 'Y')
      CMD_QUERY_GETROTATION.echoReturn(stepperYAxis.read());
    else if (arguments[0].letter == 'Z')
      CMD_QUERY_GETROTATION.echoReturn(stepperZAxis.read());
  });

  CMD_QUERY_GETLIMITSWITCHSTATUS = Command(301, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'Y')
      CMD_QUERY_GETLIMITSWITCHSTATUS.echoReturn(stepperYAxisLimitswitch.isPressed());
    else if (arguments[0].letter == 'Z')
      CMD_QUERY_GETLIMITSWITCHSTATUS.echoReturn(stepperZAxisLimitswitch.isPressed());
  });

  CMD_CONFIGURATION_DELAY        = Command(400, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'Y')
      stepperYAxis.setPulseDelay(arguments[0].number);
    else if (arguments[0].letter == 'Z')
      stepperZAxis.setPulseDelay(arguments[0].number);
  });

  CMD_CONFIGURATION_STEPSPERREV  = Command(401, 1, [](unsigned int argumentsCount, Command::argumentT *arguments) {
    if (arguments[0].letter == 'Y')
      stepperYAxis.setStepsPerRevolution(arguments[0].number);
    else if (arguments[0].letter == 'Z')
      stepperZAxis.setStepsPerRevolution(arguments[0].number);
  });
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, LOW);
    usbSerialCommand = Serial.readString();

    c = 0;
    while (!availableCommands[c]->parse(usbSerialCommand) && c < COMMANDS) {
      c++;
    }

    if (c < COMMANDS)
      availableCommands[c]->echo();

    Serial.flush();
  }
  else
    digitalWrite(LED_BUILTIN, HIGH);
}
