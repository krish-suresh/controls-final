#define MOTOR_IN1 6
#define MOTOR_IN2 5
#define SENSOR_PIN A0

int pwm1 = 200;
int pwm2 = 0;

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
int sensor = 0;
void loop() {
  // analogWrite(MOTOR_IN2, pwm2);
  // pwm2 = (pwm2 + 1 - 160) % 80 + 160; 
  sensor = 512 - analogRead(SENSOR_PIN) - (pwm2 * -0.86462046 + 118);
  sensor = constrain(sensor, 1, 5000);
  Serial.print(pwm2);
  Serial.print(",");
  Serial.print(sensor);
  Serial.print(",");
  Serial.println(1.0/sqrt(sensor), 5);
  delay(50);
  // if (Serial.available() > 0) {
  //   pwm1 = Serial.read();
  //   pwm2 = Serial.read();
  //   if (pwm1 == 0) {
  //     digitalWrite(MOTOR_IN1, LOW);      
  //   } else {
  //     analogWrite(MOTOR_IN1, pwm1);
  //   }
  //   if (pwm2 == 0) {
  //     digitalWrite(MOTOR_IN2, LOW);      
  //   } else {
  //     analogWrite(MOTOR_IN2, pwm2);
  //   }

  //   delay(100);
  //   Serial.println(analogRead(SENSOR_PIN));
  // }
}
