#define USE_ADC_V2 true

#define MOTOR_IN1 11
#define MOTOR_IN2 3

#if USE_ADC_V2
  #include <directADC.h>
  #define SENSOR_PIN ADC_A0
#else
  #define SENSOR_PIN A0
#endif

#include "sensor_data.h"

#define PWM_MID_POINT 150
#define DISTANCE_SETPOINT 0.013

#define MAG_LINE_FIT_SLOPE 0.8447
#define MAG_LINE_FIT_YINT 400

#define K_P 1000.0
#define K_I 0.0
#define K_D 1.0

#define SERIAL_PRINT false

int sensor = 0;
int sensor_raw = 0;
int pwm = PWM_MID_POINT;

unsigned long times[100] = { 0 };
int num_times = 0;
unsigned long prevTime = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;
double errorSum = 0;


void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 0;
  OCR2B = 0;

#if USE_ADC_V2
  ADC_enable();
  // This might be too low of a prescaler
  ADC_setPrescaler(32);
  ADC_setReference(ADC_VCC);
  setAnalogMux(SENSOR_PIN);
  ADC_setResolution(10);
#endif

  delay(50);
  prevTime = micros();
}

void loop() {

#if USE_ADC_V2
  ADC_startConvert();
  while(!ADC_available());
  sensor_raw = ADC_read();
  sensor = sensor_raw - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#else
  int sensor_raw = analogRead(SENSOR_PIN);
  sensor = sensor_raw - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#endif
  sensor = (sensor) / 2; 
  sensor = constrain(sensor, 0, 256);
  
  double error = SENSOR_LOOKUP_TABLE[sensor] - DISTANCE_SETPOINT;
  
  unsigned long curTime = micros();

  double timeDelta = (curTime - prevTime)*1E-6;
  errorSum += timeDelta*error;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + K_D * (error - prevError)/ timeDelta, 0, 255);
  OCR2A = pwm;


  if (SERIAL_PRINT && millis() - lastPrintTime > 200) {
    Serial.print(SENSOR_LOOKUP_TABLE[sensor], 5);
    Serial.print(",");
    Serial.print(error, 5);
    Serial.print(",");
    // Serial.print(K_I*errorSum, 1);
    // Serial.print(",");
    Serial.println(pwm);
    // Serial.print(",");
    // Serial.print(sensor_raw);
    // Serial.print(",");
    // Serial.print((pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.println(sensor);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  prevError = error;
}
