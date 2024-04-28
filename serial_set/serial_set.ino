#define SENSOR_PIN A0


// Matching the frequency of the Teensy
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);

  OCR2A = 200;
  OCR2B = 0;

  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADMUX |= (1 << REFS0);

  delay(50);
}

void loop() {
  if (Serial.available() > 0) {
    OCR2A = Serial.read();

    delay(300);
    ADCSRA |= (1 << ADSC);
    while(!((ADCSRA >> ADIF) & 1)) {}
    Serial.println(ADC);
  }
}
