// Lab 3 - Step 2: DC Motor Test with L293D
// Runs motor forward and backward at set PWM speed
// yluo / Shwin

const int ENA = 5;   // L293D enable (PWM)
const int IN1 = 7;   // L293D input 1
const int IN2 = 8;   // L293D input 2

int pwmSpeed = 192;  // 0–255 (≈3/4 speed)

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  Serial.begin(9600);
  Serial.println("DC Motor Step 2 Test Starting...");
}

void loop() {
  // Forward
  Serial.println("Forward...");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwmSpeed);
  delay(5000);

  // Stop
  Serial.println("Stop...");
  analogWrite(ENA, 0);
  delay(1000);

  // Reverse
  Serial.println("Reverse...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwmSpeed);
  delay(5000);

  // Stop again
  Serial.println("Stop...");
  analogWrite(ENA, 0);
  delay(1000);
}
