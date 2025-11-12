// Lab 3 - Step 2 (Your Pin Map)
// DC Motor Test with L293D
// Arduino: D5 -> L293D pin1 (EN1,2)
//           D4 -> L293D pin2 (IN1)
//           D3 -> L293D pin7 (IN2)

const int ENA = 5;  // PWM speed control
const int IN1 = 4;  // Direction control 1
const int IN2 = 3;  // Direction control 2

int pwmSpeed = 192; // 0–255 (≈3/4 speed)

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lab 3 - Step 2: DC Motor Test");
}

void loop() {
  // Forward rotation
  Serial.println("Motor: FORWARD");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwmSpeed);
  delay(5000);

  // Stop
  Serial.println("Motor: STOP");
  analogWrite(ENA, 0);
  delay(1000);

  // Reverse rotation
  Serial.println("Motor: REVERSE");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwmSpeed);
  delay(5000);

  // Stop again
  Serial.println("Motor: STOP");
  analogWrite(ENA, 0);
  delay(1000);
}
