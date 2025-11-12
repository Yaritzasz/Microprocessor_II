/*
  Lab 3 – Step 3: DS1307 RTC + DC Motor Integration
  ----------------------------------------------
  Hardware mapping (confirmed from photo and lab):
    D5 → L293D pin 1 (Enable 1,2 – PWM)
    D4 → L293D pin 2 (Input 1)
    D3 → L293D pin 7 (Input 2)
    A4 → SDA (I²C data)
    A5 → SCL (I²C clock)
    VCC → 5 V, GND → common ground (Arduino + motor supply)
*/

#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

// --- Motor pins ---
const int ENA = 5;   // PWM enable (L293D pin 1)
const int IN1 = 4;   // Direction 1 (L293D pin 2)
const int IN2 = 3;   // Direction 2 (L293D pin 7)
int pwmSpeed = 200;  // Speed (0–255)

// --- Timing ---
const unsigned long RUN_DURATION = 30000;  // 30 seconds
bool motorRunning = false;
unsigned long motorStartTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Lab 3 – Step 3: DS1307 RTC + Motor Control");

  // Motor setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  analogWrite(ENA, 0);  // motor off

  // RTC setup
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  // Uncomment only once to set the RTC to your computer time, then comment back out
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (!rtc.isrunning()) {
    Serial.println("RTC was stopped – starting clock now.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("RTC ready\n");
}

void loop() {
  DateTime now = rtc.now();

  // Display current time
  char buf[9];
  sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  Serial.print("Time: ");
  Serial.println(buf);

  // Detect minute boundary → run motor for 30 s
  if (now.second() == 0 && !motorRunning) {
    Serial.println("New minute detected → Motor ON for 30 s");
    startMotor();
    motorRunning = true;
    motorStartTime = millis();
  }

  // Stop after 30 seconds
  if (motorRunning && (millis() - motorStartTime >= RUN_DURATION)) {
    stopMotor();
    motorRunning = false;
    Serial.println("Motor OFF\n");
  }

  delay(1000);
}

// --- Helper functions ---
void startMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwmSpeed);
}

void stopMotor() {
  analogWrite(ENA, 0);
}
