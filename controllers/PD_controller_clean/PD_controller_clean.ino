#define MOTOR_IN1 11
#define MOTOR_IN2 3

#include "sensor_data.h"

#define PWM_MID_POINT 200
#define VOLTAGE_MID_POINT PWM_MID_POINT / 255 * 12.0
#define DISTANCE_SETPOINT 0.0058

#define MAG_LINE_FIT_SLOPE 0.77
#define MAG_LINE_FIT_YINT 415
#define K 1
#define K_P 45000.0 * K
#define K_I 0
#define K_D 40 * K
#define SERIAL_PRINT false

#define SENSOR_BUFFER_LEN (70)

float sensor_buffer[SENSOR_BUFFER_LEN] = { 0.0 };
int sensor_counter = 0;

int sensor = 0;
int sensor_raw = 0;
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
int pwm_avg = 0;
float alpha = 0.2;

void setup() {
  Serial.begin(500000);

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

  // ADCSRA |= (1 << ADSC);
  // while(!((ADCSRA >> ADIF) & 1)) {}
  // ADCSRA |= 1 << ADIF;
  // sensor_raw = ADC;
  sensor_raw = analogRead(A0);

  pwm_avg = (alpha * pwm) + ((1 - alpha) * pwm_avg);
  // pwm_avg = pwm;

  sensor = sensor_raw - (pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
  sensor = constrain(sensor, 0, 512);
  float sensor_lookup;
  if (sensor % 2 == 1) {
    sensor_lookup = (SENSOR_LOOKUP_TABLE[sensor / 2] + SENSOR_LOOKUP_TABLE[sensor / 2 + 1] ) / 2.0;
  } else {
    sensor_lookup = SENSOR_LOOKUP_TABLE[sensor / 2];
  }

  error = sensor_lookup - DISTANCE_SETPOINT;
  
  curTime = micros();

  timeDelta = (curTime - prevTime)*1E-6;
  

  // sensor_buffer[sensor_counter] = error;
  // sensor_counter = (sensor_counter + 1) % SENSOR_BUFFER_LEN;

  // float sensor_avg = 0;
  // for(int i = 0; i < SENSOR_BUFFER_LEN; i++) {
  //   sensor_avg += sensor_buffer[i];
  // }

  // sensor_avg /= SENSOR_BUFFER_LEN;

  errorSum += timeDelta*error;

  float curr_der = K_D * (error - prevError) / timeDelta;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + curr_der, 150, 255);
  // pwm++;
  // pwm = (pwm-150)%105 + 150;
  OCR2A = pwm;

  if (SERIAL_PRINT && millis() - lastPrintTime > 1) {
    Serial.print("P:");
    Serial.print(K_P * error, 5);
    Serial.print(",D:");
    Serial.print(curr_der, 5);
    Serial.print(",");
    Serial.print("pwm:");
    Serial.print(pwm - PWM_MID_POINT);
    Serial.print(",");
    Serial.print("sensor_lookup:");
    Serial.print(sensor_lookup, 5);
    Serial.print(",");
    Serial.print("error:");
    Serial.print(error, 5);
    Serial.print(",");
    Serial.print("pwm_cancel:");
    Serial.print((pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    Serial.print(",");
    Serial.print("sensor_raw:");
    Serial.print(sensor_raw);
    // Serial.print(pwm_old);
    // Serial.print(",");
    // Serial.print("pwm_avg:");
    // Serial.print((int) pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
    // Serial.print(",");
    // Serial.print(sensor_average - (pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.print(sensor_average - (PWM_MID_POINT * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.print(sensor_average - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));

    // Serial.print(",");
    // Serial.print("sensor_avg:");
    // Serial.print(sensor_buffer[0], 5);
    // Serial.print(",sensor_raw:");
    // Serial.print(sensor_raw);
    Serial.println();
    // Serial.print(",");
    // Serial.print((pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.println(sensor);
    lastPrintTime = millis();
  }
  prevTime = curTime;
  prevError = error;
}
