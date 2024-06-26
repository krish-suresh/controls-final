#define MOTOR_IN1 6
#define MOTOR_IN2 9
#define SENSOR_PIN A0

#define PWM_MID_POINT 190
#define ERROR_OFFSET 0.17

#define MAG_LINE_FIT_SLOPE 0.65
#define MAG_LINE_FIT_YINT 443

#define K_P 800.0
#define K_I 0
#define K_D 0
#define SERIAL_PRINT true

int sensor = 0;
int pwm = PWM_MID_POINT;


unsigned long prevTime = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;
double errorSum = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  analogWrite(MOTOR_IN1, PWM_MID_POINT);
  analogWrite(MOTOR_IN2, 0);
  delay(50);
  prevTime = micros();
}

void loop() {
  int sensor_raw = analogRead(SENSOR_PIN);
  sensor = sensor_raw - (MAG_LINE_FIT_SLOPE * pwm + MAG_LINE_FIT_YINT);
  sensor = constrain(sensor, 1, 5000);
  double error = (1.0/sqrt(sensor)) - ERROR_OFFSET; // TODO Add Setpoint
  
  unsigned long curTime = micros();

  double timeDelta = (curTime - prevTime)*1E-6;
  errorSum += timeDelta*error;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + K_D * (error - prevError)/ timeDelta, 0, 254);
  analogWrite(MOTOR_IN1, pwm);

  if (SERIAL_PRINT && millis() - lastPrintTime > 50) {
    Serial.print(error, 5);
    Serial.print(",");
    Serial.print(pwm);
    Serial.print(",");
    Serial.println(sensor);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  prevError = error;

}
