#define USE_ADC_V2 true
#define USE_PWM_V2 true

#if USE_ADC_V2
#include <directADC.h>
#endif

#if USE_PWM_V2
#include "AVR_PWM.h"
#endif

#define MOTOR_IN1 6
#define MOTOR_IN2 9

#if USE_ADC_V2
#define SENSOR_PIN ADC_A0
#else
#define SENSOR_PIN A0
#endif

#if USE_PWM_V2
AVR_PWM* PWM_Instance;
float frequency = 4482;
#endif

#define PWM_MID_POINT 190
#define ERROR_OFFSET 0.17

#define MAG_LINE_FIT_SLOPE 0.71
#define MAG_LINE_FIT_YINT 419

#define K_P 800
#define K_I 0
#define K_D 0.17

#define SERIAL_PRINT true

int sensor = 0;
float pwm = PWM_MID_POINT * 100 / 255;

// unsigned long times[100] = { 0 };
// int num_times = 0;
unsigned long prevTime = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;
double errorSum = 0;


void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

#if USE_PWM_V2
  PWM_Instance = new AVR_PWM(MOTOR_IN1, frequency, pwm);
  analogWrite(MOTOR_IN2, 0);
#endif

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

#if USE_ADC_V2
  ADC_startConvert();
  while(!ADC_available());
  sensor = ADC_read() - 512 + (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#else
  sensor = analogRead(SENSOR_PIN) - 512 + (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#endif

  int sensor_raw = analogRead(SENSOR_PIN);
  sensor = sensor_raw - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
  sensor = constrain(sensor, 1, 5000);
  double error = (1.0/sqrt(sensor)) - ERROR_OFFSET; // TODO Add Setpoint
  
  unsigned long curTime = micros();

  double timeDelta = (curTime - prevTime)*1E-6;
  errorSum += timeDelta*error;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + K_D * (error - prevError)/ timeDelta, 0, 254);

#if USE_PWM_V2
  float pwm_cmd = pwm * 100 / 255;
  PWM_Instance->setPWM(MOTOR_IN1, frequency, pwm_cmd);
#else
  analogWrite(MOTOR_IN1, pwm);
#endif

  if (SERIAL_PRINT && millis() - lastPrintTime > 200) {
    Serial.print(error, 5);
    Serial.print(",");
    Serial.print(K_I*errorSum, 1);
    Serial.print(",");
    Serial.println(pwm);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  prevError = error;
}
