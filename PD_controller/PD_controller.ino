#define USE_ADC_V2 true

#if USE_ADC_V2
#include <directADC.h>
#endif

#define MOTOR_IN1 6
#define MOTOR_IN2 9
#if USE_ADC_V2
#define SENSOR_PIN ADC_A0
#else
#define SENSOR_PIN A0
#endif

#define PWM_MID_POINT 190
#define ERROR_OFFSET 0.11

#define MAG_LINE_FIT_SLOPE -0.65
#define MAG_LINE_FIT_YINT -69

#define K_P .0
#define K_I 0
#define K_D 0
#define SERIAL_PRINT true

int sensor = 0;
int pwm = PWM_MID_POINT;


unsigned long prevTime = 0;
unsigned long times[100] = { 0 };
int num_times = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;
double errorSum = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  analogWrite(MOTOR_IN1, PWM_MID_POINT);
  analogWrite(MOTOR_IN2, 0);
#if USE_ADC_V2
  ADC_enable();
  // This might be too low of a prescaler
  ADC_setPrescaler(16);
  ADC_setReference(ADC_VCC);
  setAnalogMux(SENSOR_PIN);
  ADC_setResolution(10);
#endif
  delay(50);
  prevTime = micros();
}

unsigned long t0_start = 0;
unsigned long t0_end = 0;
unsigned long t1_end = 0;
unsigned long t2_end = 0;
unsigned long t3_end = 0;
unsigned long t4_end = 0;
unsigned long t5_end = 0;
unsigned long t6_end = 0;
unsigned long t7_end = 0;
unsigned long t8_end = 0;
unsigned long t9_end = 0;

void loop() {
  // if (num_times < 100) {
  //   times[num_times] = micros();
  //   num_times++;
  // } else {
  //   num_times = 0;
  //   for (int i = 0; i < 99; i++) {
  //     Serial.println(times[i+1] - times[i]);
  //   }
  // }
  // t0_start = micros();
#if USE_ADC_V2
  ADC_startConvert();
  while(!ADC_available());
  sensor = ADC_read() - 512 + (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#else
  sensor = analogRead(SENSOR_PIN) - 512 + (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#endif

  // t0_end = micros();
  sensor = constrain(sensor, 1, 5000);
  // t1_end = micros();
  // double error = (1.0/pow(sensor, 0.5)) - ERROR_OFFSET; // TODO Add Setpoint
  double error = (1.0/sqrt(sensor)) - ERROR_OFFSET; // TODO Add Setpoint
  // t2_end = micros();
  
  unsigned long curTime = micros();
  // t3_end = micros();

  double timeDelta = (curTime - prevTime)*1E-6;
  // t4_end = micros();
  errorSum += timeDelta*error;
  // t5_end = micros();

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + K_D * (error - prevError)/ timeDelta, 0, 254);
  // t6_end = micros();
  analogWrite(MOTOR_IN1, pwm);
  // t7_end = micros();

  if (SERIAL_PRINT && millis() - lastPrintTime > 200) {
    Serial.print(error, 5);
    Serial.print(",");
    Serial.print(K_I*errorSum, 1);
    Serial.print(",");
    Serial.print(pwm);
    Serial.print(",");
    Serial.println(sensor);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  // t8_end = micros();
  prevError = error;
  // t9_end = micros();

  // Serial.print("t0: ");
  // Serial.println(t0_end - t0_start);
  // Serial.print("t1: ");
  // Serial.println(t1_end - t0_end);
  // Serial.print("t2: ");
  // Serial.println(t2_end - t1_end);
  // Serial.print("t3: ");
  // Serial.println(t3_end - t2_end);
  // Serial.print("t4: ");
  // Serial.println(t4_end - t3_end);
  // Serial.print("t5: ");
  // Serial.println(t5_end - t4_end);
  // Serial.print("t6: ");
  // Serial.println(t6_end - t5_end);
  // Serial.print("t7: ");
  // Serial.println(t7_end - t6_end);
  // Serial.print("t8: ");
  // Serial.println(t8_end - t7_end);
  // Serial.print("t9: ");
  // Serial.println(t9_end - t8_end);
}
