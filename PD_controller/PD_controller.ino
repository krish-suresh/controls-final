#define MOTOR_IN1 6
#define MOTOR_IN2 5
#define SENSOR_PIN A0

#define PWM_MID_POINT 190
#define ERROR_OFFSET 0.046

int sensor = 0;
int pwm = 0;

unsigned long prevTime = 0;
double prevError = 0;


double K_P = 500;
double K_D = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  analogWrite(MOTOR_IN2, PWM_MID_POINT);
  delay(50);
  prevTime = micros();
}

void loop() {

  sensor = analogRead(SENSOR_PIN);
  // TODO convert
  double error = (1.0/sqrt(sensor)) - ERROR_OFFSET; // TODO Add Setpoint
  
  unsigned long curTime = micros();

  double timeDelta = (curTime - prevTime)/1E-6;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_D * (error - prevError)/ timeDelta, 0, 254);
  analogWrite(MOTOR_IN2, pwm);
  // Serial.print(error, 5);
  // Serial.print(",");
  // Serial.println(pwm);
  prevTime = curTime;
  prevError = error;

  // delay(50);

}
