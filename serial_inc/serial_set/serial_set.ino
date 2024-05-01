#define SENSOR_PIN A0

#define PWM_START 150
#define INC 5

bool read = false;
int start = 0;
int pwm = PWM_START;
// Matching the frequency of the Teensy
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);

  OCR2A = pwm;
  OCR2B = 0;
  Serial.println(pwm);

  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    Serial.read();
    pwm += INC;
    OCR2A = pwm;
    Serial.println(pwm);
    // read = true;
    // start = millis();
  }

  // if (read && millis() - start > 200) {
  //   int sensorValue = analogRead(SENSOR_PIN);

  //   byte highByte = (sensorValue >> 8) & 0xFF;
  //   byte lowByte = sensorValue & 0xFF;
  //   Serial.write(highByte);
  //   Serial.write(lowByte);

  //   read = false;
  // }
}
