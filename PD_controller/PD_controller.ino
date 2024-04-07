#define MOTOR_IN1 6
#define MOTOR_IN2 5
#define SENSOR_PIN A0

#define PWM_MID_POINT 200
#define ERROR_OFFSET 0.2

#define MAG_LINE_FIT_SLOPE -0.86462046
#define MAG_LINE_FIT_YINT 118

#define K_P 600.0
#define K_D 0.17
//1000 .445
#define SERIAL false

int sensor = 0;
int pwm = PWM_MID_POINT;


unsigned long prevTime = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, PWM_MID_POINT);
  delay(50);
  prevTime = micros();
}

void loop() {
  sensor = 512 - analogRead(SENSOR_PIN) + (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
  sensor = constrain(sensor, 1, 5000);
  double error = (1.0/sqrt(sensor)) - ERROR_OFFSET; // TODO Add Setpoint
  
  unsigned long curTime = micros();

  double timeDelta = (curTime - prevTime)*1E-6;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_D * (error - prevError)/ timeDelta, 0, 254);
  analogWrite(MOTOR_IN1, pwm);

  if (SERIAL && millis() - lastPrintTime > 50) {
    Serial.print(error, 5);
    Serial.print(",");
    Serial.println(pwm);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  prevError = error;

}
