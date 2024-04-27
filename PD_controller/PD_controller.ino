#define USE_ADC_V2 false
#define USE_PWM_V2 false

#if USE_ADC_V2
#include <directADC.h>
#endif

#if USE_PWM_V2
#include "AVR_PWM.h"
#endif

#define MOTOR_IN1 9
#define MOTOR_IN2 6

#if USE_ADC_V2
#define SENSOR_PIN ADC_A0
#else
#define SENSOR_PIN A0
#endif

#if USE_PWM_V2
AVR_PWM* PWM_Instance;
float frequency = 250;
#endif

#define PWM_MID_POINT 190
#define ERROR_OFFSET 0.17

#define MAG_LINE_FIT_SLOPE 0.71
#define MAG_LINE_FIT_YINT 419

#define K_P 800
#define K_I 0
#define K_D .17

#define SERIAL_PRINT true

int sensor = 0;
int sensor_raw = 0;
float pwm = PWM_MID_POINT * 100 / 255;

unsigned long times[100] = { 0 };
int num_times = 0;
unsigned long prevTime = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;
double errorSum = 0;


void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN2, OUTPUT);

  analogWrite(MOTOR_IN2, 0);
#if USE_PWM_V2
  PWM_Instance = new AVR_PWM(MOTOR_IN1, frequency, pwm);
#else
  pinMode(MOTOR_IN1, OUTPUT);
#endif

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
  if (num_times < 100) {
    times[num_times] = micros();
    num_times++;
  } else {
    num_times = 0;
    for (int i = 0; i < 99; i++) {
      Serial.println(times[i+1] - times[i]);
    }
  }

#if USE_ADC_V2
  ADC_startConvert();
  while(!ADC_available());
  sensor_raw = ADC_read();
  sensor = sensor_raw - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#else
  int sensor_raw = analogRead(SENSOR_PIN);
  sensor = sensor_raw - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
#endif

  sensor = constrain(sensor, 1, 5000);
  double error = (1.0/sqrt(sensor)) - ERROR_OFFSET; // TODO Add Setpoint
  
  unsigned long curTime = micros();

  double timeDelta = (curTime - prevTime)*1E-6;
  errorSum += timeDelta*error;

  // pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + K_D * (error - prevError)/ timeDelta, 0, 254);
  pwm = constrain(PWM_MID_POINT + K_P * error + K_D * (error - prevError)/ timeDelta, 0, 254);

#if USE_PWM_V2
  uint8_t pwm_cmd = (uint8_t)(pwm * 100 / 255);
  float dutyCycle = 75;
  PWM_Instance->setPWM(MOTOR_IN1, frequency, dutyCycle);
#else
  analogWrite(MOTOR_IN1, 123);
#endif

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
  prevError = error;
}
