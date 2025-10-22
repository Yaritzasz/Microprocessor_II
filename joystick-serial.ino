#include <Arduino.h>

const int SW_pin = 2;   // joystick button (not required for movement)
const int X_pin  = A0;  // joystick X-axis
const int Y_pin  = A1;  // joystick Y-axis
const int buzzer_pin = 12;

void beep() {
  digitalWrite(buzzer_pin, HIGH);
  delay(100);
  digitalWrite(buzzer_pin, LOW);
}

void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int X_val = analogRead(X_pin);
  int Y_val = analogRead(Y_pin);

  // center region dead zone (joystick rest area)
  int deadZone = 200;

  // send direction to host (only when clearly tilted)
  if (X_val < 512 - deadZone)
    Serial.write('a');   // left
  else if (X_val > 512 + deadZone)
    Serial.write('d');   // right
  else if (Y_val < 512 - deadZone)
    Serial.write('s');   // down
  else if (Y_val > 512 + deadZone)
    Serial.write('w');   // up

  // check if host signals that an apple was eaten
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'E') beep();
  }

  delay(100);
}
