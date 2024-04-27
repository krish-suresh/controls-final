#include "AVR_PWM.h"

#define MOTOR_IN1 9
#define MOTOR_IN2 10
#define SENSOR_PIN A0


// Matching the frequency of the Teensy
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);

  OCR2A = 0;
  OCR2B = 0;

  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    OCR2A = Serial.read();

    delay(300);
    Serial.println(analogRead(SENSOR_PIN));
  }
}
