#define MOTOR_IN1 11
#define MOTOR_IN2 3
#define SENSOR_PIN A0

int pwm1 = 200;
int pwm2 = 0;

#define BUFFER_LEN (20)
int reading_buffer[BUFFER_LEN] = { 0 };
int reading_counter = 0;

void setup() {
  Serial.begin(115200);
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
  int sensor_raw = analogRead(SENSOR_PIN);
  sensor = sensor_raw - 512 + (pwm1 * - 0.65 - 69);
  sensor = constrain(sensor, 1, 5000);
  reading_buffer[reading_counter] = sensor_raw;
  reading_counter++;
  if (reading_counter == BUFFER_LEN) {
    reading_counter = 0;
  }
  int sensor_avg = 0;
  for (int i = 0; i< BUFFER_LEN; i++) {
    sensor_avg += reading_buffer[i];
  }
  sensor_avg /= BUFFER_LEN;
  Serial.print(sensor_avg);
  Serial.print(",");
  Serial.print(pwm1);
  Serial.print(",");
  Serial.print(sensor);
  Serial.print(",");
  Serial.println(1.0/(sqrt(sensor)), 5);
  delay(50);
}
