#define MOTOR_IN1 11
#define MOTOR_IN2 3

#include "sensor_data.h"

#define PWM_MID_POINT 200
#define VOLTAGE_MID_POINT PWM_MID_POINT / 255 * 12.0
#define DIST_EM_TO_PM 0.004
#define DIST_EM_OFFSET 0.0
#define DISTANCE_SETPOINT (DIST_EM_TO_PM - DIST_EM_OFFSET)

#define MAG_LINE_FIT_SLOPE 0.79
#define MAG_LINE_FIT_YINT 408
#define K .4
#define K_P 85000.0 * K
#define K_I 0
#define K_D 150 * K
#define SERIAL_PRINT false

#define SENSOR_BUFFER_LEN (10)
#define PWM_BUFFER 107
#define PWM_AVG 5
#define DER_BUFFER_LEN 1

#define MAX_COUNT 100

int sensor = 0;
int sensor_raw = 0;
int sensor_buffer[SENSOR_BUFFER_LEN] = { 0 };
int pwm_buffer[PWM_BUFFER] = { 0 };
int sensor_counter = 0;
long sensor_average = 0;
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
int pwm_avg = 0;
int pwm_counter = 0;
int pwm_old = pwm;
int counter = 0;
float alpha = 0.15;

float d_error_buffer[DER_BUFFER_LEN] = { 0 };
int d_error_counter = 0;
int max_count = 0;

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

  ADCSRA |= (1 << ADSC);
  while(!((ADCSRA >> ADIF) & 1)) {}
  ADCSRA |= 1 << ADIF;
  sensor_raw = ADC;

  // sensor_buffer[sensor_counter] = sensor_raw;
  // sensor = constrain(sensor, 0, 254);

  // sensor_counter++;
  // if (sensor_counter == SENSOR_BUFFER_LEN) {
  //   sensor_counter = 0;
  // }

  // sensor_average = 0;
  // for (int i = 0; i < SENSOR_BUFFER_LEN; i++) {
  //   sensor_average += sensor_buffer[i];
  // }
  // sensor_average /= SENSOR_BUFFER_LEN;

  // pwm_old = pwm_buffer[pwm_counter];

  // long pwm_sum = 0;
  // for (int i =0;i < PWM_AVG;i++) {
  //   pwm_sum += pwm_buffer[(i+pwm_counter) >= PWM_BUFFER ? i+pwm_counter-PWM_BUFFER : (i+pwm_counter)];
  //   // pwm_sum += pwm_buffer[(i+pwm_counter) % PWM_BUFFER];
  // }
  // pwm_avg = pwm_sum/PWM_AVG;
  pwm_avg = (alpha * pwm) + ((1 - alpha) * pwm_avg);

  // pwm_buffer[pwm_counter] = pwm;

  // pwm_counter++;
  // if (pwm_counter == PWM_BUFFER) {
  //   pwm_counter = 0;
  // }

  sensor = sensor_raw - (pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
  // sensor = (sensor) / 2; 
  sensor = constrain(sensor, 0, 512);
  float sensor_lookup;
  if (sensor % 2 == 1) {
    sensor_lookup = (SENSOR_LOOKUP_TABLE[sensor / 2] + SENSOR_LOOKUP_TABLE[sensor / 2 + 1] ) / 2.0;
  } else {
    sensor_lookup = SENSOR_LOOKUP_TABLE[sensor / 2];
  }
  error = sensor_lookup - DISTANCE_SETPOINT;
  // error = SENSOR_LOOKUP_TABLE[sensor] - DISTANCE_SETPOINT;
  
  curTime = micros();

  timeDelta = (curTime - prevTime)*1E-6;
  errorSum += timeDelta*error;

  float curr_der = K_D * (error - prevError) / timeDelta;
  // d_error_buffer[d_error_counter] = curr_der;
  // d_error_counter++;
  // if (d_error_counter == DER_BUFFER_LEN) {
  //   d_error_counter = 0;
  // }
  
  // for (int i = 0; i < DER_BUFFER_LEN; i++) {
  //   curr_der += d_error_buffer[i];
  // }
  // curr_der /= DER_BUFFER_LEN;

  pwm = constrain(PWM_MID_POINT + K_P * error + K_I*errorSum + curr_der, 150, 255);
  // if (abs(error) < 0.00005)
  //   pwm = PWM_MID_POINT;
  // pwm++;
  // pwm = (pwm-150)%105 + 150;
  // if (counter > 80) {
  //   pwm = 180;
  // } else {
  //   pwm = 255;
  // }
  // counter = (counter+1)%MAX_COUNT;
  OCR2A = pwm;
  // Serial.println(sensor_lookup, 5);
  if (SERIAL_PRINT && millis() - lastPrintTime > 1) {
    // Serial.print(curr_der, 5);
    // Serial.print(",");
    // Serial.print(avg_error, 5);
    // Serial.print(",");
    Serial.print("sensor:");
    Serial.print(sensor_lookup, 5);
    Serial.print(",");
    Serial.print("error:");
    Serial.print(error, 5);
    Serial.print(",");

    Serial.print("pwm:");
    Serial.print(pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
    Serial.print(",");
    // Serial.print(pwm_old);
    // Serial.print(",");
    Serial.print("pwm_avg:");
    Serial.print((int) pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT);
    // Serial.print(",");
    // Serial.print(sensor_average - (pwm_avg * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.print(sensor_average - (PWM_MID_POINT * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));
    // Serial.print(",");
    // Serial.print(sensor_average - (pwm * MAG_LINE_FIT_SLOPE + MAG_LINE_FIT_YINT));

    Serial.print(",");
    Serial.print("sensor_avg:");
    Serial.print(sensor_average);
    Serial.print(",");
    Serial.print("sensor_raw:");
    Serial.print(sensor_raw);
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
