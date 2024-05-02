#define MOTOR_IN1 11
#define MOTOR_IN2 3

#include "sensor_data.h"

#define PWM_MID_POINT 226
#define VOLTAGE_MID_POINT PWM_MID_POINT / 255 * 12.0
#define DIST_EM_TO_PM 0.0105
#define DIST_EM_OFFSET 0.00232
#define DISTANCE_SETPOINT (DIST_EM_TO_PM - DIST_EM_OFFSET)

#define MAG_LINE_FIT_SLOPE 0.7825
#define MAG_LINE_FIT_YINT 407
#define K 1
#define K_P 100000.0 * K
#define K_I 0
#define K_D 0 * K

#define SERIAL_PRINT true

#define SENSOR_BUFFER_LEN (10)

int sensor = 0;
int sensor_raw = 0;
int sensor_buffer[SENSOR_BUFFER_LEN] = { 0 };
int sensor_counter = 0;
int sensor_average = 0;
int pwm = PWM_MID_POINT;

unsigned long times[100] = { 0 };
int num_times = 0;
unsigned long prevTime = 0;
double prevError = 0;
unsigned long lastPrintTime = 0;
double errorSum = 0;
double error = 0;
unsigned long curTime = 0;
double timeDelta = 0;
float v_i = 0;


void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 0;
  OCR2B = 0;

  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |(1 << ADPS0);
  ADMUX |= (1 << REFS0);

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

  ADCSRA |= (1 << ADSC);
  while(!((ADCSRA >> ADIF) & 1)) {}
  ADCSRA |= 1 << ADIF;
  sensor_raw = ADC;

  sensor_buffer[sensor_counter] = sensor_raw;
  // sensor = constrain(sensor, 0, 254);

  sensor_counter++;
  if (sensor_counter == SENSOR_BUFFER_LEN) {
    sensor_counter = 0;
  }

  sensor_average = 0;
  for (int i = 0; i < SENSOR_BUFFER_LEN; i++) {
    sensor_average += sensor_buffer[i];
  }
  sensor_average /= SENSOR_BUFFER_LEN;

  sensor = sensor_average - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
  // sensor = (sensor) / 2; 
  sensor = constrain(sensor, 0, 512);
  if (sensor % 2 == 1) {
    error = (SENSOR_LOOKUP_TABLE[sensor / 2] + SENSOR_LOOKUP_TABLE[sensor / 2 + 1] ) / 2.0 - DISTANCE_SETPOINT;
  } else {
    error = SENSOR_LOOKUP_TABLE[sensor / 2] - DISTANCE_SETPOINT;
  }
  // error = SENSOR_LOOKUP_TABLE[sensor] - DISTANCE_SETPOINT;
  
  curTime = micros();

  timeDelta = (curTime - prevTime)*1E-6;
  errorSum += timeDelta*error;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + K_D * (error - prevError)/ timeDelta, 150, 255);
  OCR2A = pwm;


  if (SERIAL_PRINT && millis() - lastPrintTime > 200) {
    // Serial.print(SENSOR_LOOKUP_TABLE[sensor], 5);
    // Serial.print(",");
    // Serial.print((error - prevError)/ timeDelta, 5);
    // Serial.print(",");
    Serial.print(error, 5);
    Serial.print(",");
    // Serial.print(K_I*errorSum, 1);
    // Serial.print(",");
    Serial.print(pwm);
    Serial.print(",");
    // Serial.print(sensor);
    // Serial.print(",");
    // Serial.print(sensor_average);
    // Serial.print(",");
    Serial.println(sensor_raw);
    // Serial.print(",");
    // Serial.print((pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.println(sensor);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  prevError = error;
}
