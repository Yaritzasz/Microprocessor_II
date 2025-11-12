#include <Wire.h>

const int MPU_addr = 0x68;  // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);      // PWR_MGMT_1 register
  Wire.write(0);         // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);                // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);  // request a total of 14 registers

  AcX = Wire.read() << 8 | Wire.read();  // ACCEL_XOUT_H & L
  AcY = Wire.read() << 8 | Wire.read();  // ACCEL_YOUT_H & L
  AcZ = Wire.read() << 8 | Wire.read();  // ACCEL_ZOUT_H & L
  Tmp = Wire.read() << 8 | Wire.read();  // TEMP_OUT_H & L
  GyX = Wire.read() << 8 | Wire.read();  // GYRO_XOUT_H & L
  GyY = Wire.read() << 8 | Wire.read();  // GYRO_YOUT_H & L
  GyZ = Wire.read() << 8 | Wire.read();  // GYRO_ZOUT_H & L

  Serial.print(" | AcX = "); Serial.println(AcX);
  Serial.print(" | AcY = "); Serial.println(AcY);
  Serial.print(" | AcZ = "); Serial.println(AcZ);
  Serial.print(" | Tmp = "); Serial.println(Tmp / 340.00 + 36.53); // °C formula
  Serial.print(" | GyX = "); Serial.println(GyX);
  Serial.print(" | GyY = "); Serial.println(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  Serial.println();

  delay(333);
}
