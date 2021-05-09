#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define USB_SERIAL_SPEED               115200      // Velocidad del puerto serie en baudios

#define STEPPER_Z_AXIS_ENGLAND_MODE         false  // Funcionamiento del motor paso a paso del eje de rotación Z al derecho o al revés
#define STEPPER_Z_AXIS_PIN_EN               2      // Pin ENABLE del motor paso a paso del eje de rotación Z
#define STEPPER_Z_AXIS_PIN_STEP             3      // Pin STEP del motor paso a paso del eje de rotación Z
#define STEPPER_Z_AXIS_PIN_DIR              4      // Pin DIR del motor paso a paso del eje de rotación Z
#define STEPPER_Z_AXIS_PIN_LIMITSWITCH     11      // Pin final de carrera del eje de rotación Z
#define STEPPER_Z_AXIS_RESOLUTION         200      // Resolución motor paso a paso eje de rotación Z
#define STEPPER_Z_AXIS_HARDWARE_REDUCTION   8      // Reducción driver A4988 del motor paso a paso eje de rotación Z
#define STEPPER_Z_AXIS_PHYSICAL_REDUCTION   3      // Reducción física del motor paso a paso eje de rotación Z

#define STEPPER_Y_AXIS_ENGLAND_MODE         true   // Funcionamiento del motor paso a paso del eje de rotación Y al derecho o al revés
#define STEPPER_Y_AXIS_PIN_EN               5      // Pin ENABLE del motor paso a paso del eje de rotación Y
#define STEPPER_Y_AXIS_PIN_STEP             6      // Pin STEP del motor paso a paso del eje de rotación Y
#define STEPPER_Y_AXIS_PIN_DIR              7      // Pin DIR del motor paso a paso del eje de rotación Y
#define STEPPER_Y_AXIS_PIN_LIMITSWITCH     12      // Pin final de carrera del eje de rotación Y
#define STEPPER_Y_AXIS_RESOLUTION         200      // Resolución motor paso a paso eje de rotación Y
#define STEPPER_Y_AXIS_HARDWARE_REDUCTION  16      // Reducción driver A4988 del motor paso a paso eje de rotación Y
#define STEPPER_Y_AXIS_PHYSICAL_REDUCTION   3      // Reducción física del motor paso a paso eje de rotación Y

#define SERVO_Y_AXIS                       10

#endif
