#define MOTOR_IN1 6
#define MOTOR_IN2 9
#define SENSOR_PIN A0

int pwm1 = 170;
int pwm2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  // analogWrite(MOTOR_IN1, pwm1);
  // analogWrite(MOTOR_IN2, pwm2);
  delay(50);
}
int sensor = 0;
void loop() {
  analogWrite(MOTOR_IN1, pwm1);
  // pwm1 = (pwm1 + 1 - 160) % 80 + 160; 
  sensor = analogRead(SENSOR_PIN) - 512 + (pwm1 * - 0.65 - 69);
  sensor = constrain(sensor, 1, 5000);
  Serial.print(analogRead(SENSOR_PIN));
  Serial.print(",");
  Serial.print(pwm1);
  Serial.print(",");
  Serial.print(sensor);
  Serial.print(",");
  Serial.println(1.0/(sqrt(sensor)), 5);
  delay(50);
}
