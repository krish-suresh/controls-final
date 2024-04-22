#include "AVR_PWM.h"

#define MOTOR_IN1 9
#define MOTOR_IN2 10
#define SENSOR_PIN A0

AVR_PWM* PWM1_Instance;
AVR_PWM* PWM2_Instance;
int pwm1_dc = 0;
int pwm2_dc = 0;

// Matching the frequency of the Teensy
float frequency = 4482;
float dutyCycle;

void setup() {
  Serial.begin(9600);
  PWM1_Instance = new AVR_PWM(MOTOR_IN1, frequency, pwm1_dc);
  analogWrite(MOTOR_IN2, 0);
  // PWM2_Instance = new AVR_PWM(MOTOR_IN2, frequency, pwm2_dc);
  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    pwm1_dc = Serial.read();
  //   pwm2_dc = Serial.read();
    PWM1_Instance->setPWM(MOTOR_IN1, frequency, pwm1_dc);
    // PWM2_Instance->setPWM(MOTOR_IN2, frequency, 25);

    delay(300);
    Serial.println(analogRead(SENSOR_PIN));
  }
}
