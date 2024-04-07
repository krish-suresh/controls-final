#define MOTOR_IN1 6
#define MOTOR_IN2 5
#define SENSOR_PIN A0

int pwm1 = 0;
int pwm2 = 200;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  // digitalWrite(MOTOR_IN1, LOW);
  // digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, pwm1);
  analogWrite(MOTOR_IN2, pwm2);
  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    pwm1 = Serial.read();
    pwm2 = Serial.read();
    if (pwm1 == 0) {
      digitalWrite(MOTOR_IN1, LOW);      
    } else {
      analogWrite(MOTOR_IN1, pwm1);
    }
    if (pwm2 == 0) {
      digitalWrite(MOTOR_IN2, LOW);      
    } else {
      analogWrite(MOTOR_IN2, pwm2);
    }

    delay(400);
    Serial.println(analogRead(SENSOR_PIN));
  }
}
