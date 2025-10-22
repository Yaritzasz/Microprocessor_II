#include <Wire.h>

const int MPU_addr = 0x68;  // I2C address of the MPU-6050
const int buzzer_pin = 12;

// Raw gyro values
int16_t GyX, GyY, GyZ;

// Dead zone threshold to prevent noise
const int threshold = 3000;

void beep() {
  digitalWrite(buzzer_pin, HIGH);
  delay(100);
  digitalWrite(buzzer_pin, LOW);
}

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);   // Power management register
  Wire.write(0);      // Wake up MPU6050
  Wire.endTransmission(true);

  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Request gyro registers (start at 0x43)
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);  // 3 axes * 2 bytes each

  GyX = Wire.read() << 8 | Wire.read();  // X-axis rotation
  GyY = Wire.read() << 8 | Wire.read();  // Y-axis rotation
  GyZ = Wire.read() << 8 | Wire.read();  // Z-axis rotation

  // Determine direction based on gyro tilt
  // Positive X tilt → move right, Negative X tilt → move left
  // Positive Y tilt → move up, Negative Y tilt → move down
  if (GyX > threshold)
    Serial.write('d');   // tilt right
  else if (GyX < -threshold)
    Serial.write('a');   // tilt left
  else if (GyY > threshold)
    Serial.write('w');   // tilt up
  else if (GyY < -threshold)
    Serial.write('s');   // tilt down

  // Listen for Python signal to beep
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'E') beep();
  }

  delay(100);
}
