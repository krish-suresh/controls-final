// Basic sketch for trying out the Adafruit DRV8871 Breakout
#define MOTOR_IN1 6
#define MOTOR_IN2 5
#define SENSOR_PIN A0

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
}

void loop() {
Serial.println(analogRead(SENSOR_PIN));
  // ramp up forward
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, 200);
//  for (int i=0; i<255; i++) {
//    delay(10);
//    Serial.println(analogRead(SENSOR_PIN));
//
//  }
//
//  // forward full speed for one second
//  delay(1000);
//  
//  // ramp down forward
//  for (int i=255; i>=0; i--) {
//    analogWrite(MOTOR_IN2, i);
//    delay(10);
//    Serial.println(analogRead(SENSOR_PIN));
//
//  }
//
//  // ramp up backward
//  digitalWrite(MOTOR_IN2, LOW);
//  for (int i=0; i<255; i++) {
//    analogWrite(MOTOR_IN1, i);
//    delay(10);
//    Serial.println(analogRead(SENSOR_PIN));
//
//  }
//
//  // backward full speed for one second
//  delay(1000);
//
//  // ramp down backward
//  for (int i=255; i>=0; i--) {
//    analogWrite(MOTOR_IN1, i);
//    delay(10);
//    Serial.println(analogRead(SENSOR_PIN));
//  }
}
