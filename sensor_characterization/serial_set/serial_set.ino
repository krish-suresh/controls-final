#define SENSOR_PIN A0

#define PWM_START 0

bool read = false;
int start = 0;
// Matching the frequency of the Teensy
void setup() {
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);

  OCR2A = PWM_START;
  OCR2B = 0;

  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADMUX |= (1 << REFS0);

  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    OCR2A = Serial.read() + PWM_START;
    read = true;
    start = millis();
  }

  if (read && millis() - start > 2000) {
    int sensorValue = analogRead(SENSOR_PIN);

    byte highByte = (sensorValue >> 8) & 0xFF;
    byte lowByte = sensorValue & 0xFF;
    Serial.write(highByte);
    Serial.write(lowByte);

    read = false;
  }
}
