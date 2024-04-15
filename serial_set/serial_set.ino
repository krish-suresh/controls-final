#define MOTOR_IN1 6
#define MOTOR_IN2 9
#define SENSOR_PIN A0

int pwm1 = 0;
int pwm2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  analogWrite(MOTOR_IN1, pwm1);
  analogWrite(MOTOR_IN2, pwm2);
  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    pwm1 = Serial.read();
    pwm2 = Serial.read();
    analogWrite(MOTOR_IN1, pwm1);
    analogWrite(MOTOR_IN2, pwm2);

    delay(300);
    Serial.println(analogRead(SENSOR_PIN));
  }
}
