/*
  Lab 3 – Step 4: DS1307 RTC + DC Motor + Direction Button
  --------------------------------------------------------
  Hardware mapping:
    D5 → L293D pin 1 (Enable 1,2 – PWM)
    D4 → L293D pin 2 (IN1)
    D3 → L293D pin 7 (IN2)
    D2 → Pushbutton (to GND, use INPUT_PULLUP)
    A4 → SDA  (I²C data)
    A5 → SCL  (I²C clock)
    VCC → 5 V, GND → common ground (Arduino + motor supply + RTC)
*/

#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

// --- Motor pins ---
const int ENA = 5;
const int IN1 = 4;
const int IN2 = 3;

// --- Button ---
const int BTN_DIR = 2;  // button to GND
bool dirCW = true;      // true = clockwise
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 200;

// --- Speed & timing ---
int pwmSpeed = 200;
const unsigned long RUN_DURATION = 30000;  // 30 s
bool motorRunning = false;
unsigned long motorStartTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Lab 3 – Step 4: RTC + Motor + Direction Button");

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  analogWrite(ENA, 0);

  pinMode(BTN_DIR, INPUT_PULLUP);  // internal pull-up

  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("❌ RTC not found!");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC was stopped – starting clock now.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("✅ RTC ready\n");
}

void loop() {
  // --- Button handling (toggle direction) ---
  bool reading = digitalRead(BTN_DIR);
  if (reading != lastButtonState && (millis() - lastDebounce) > debounceDelay) {
    lastDebounce = millis();
    if (reading == LOW) {
      dirCW = !dirCW;
      Serial.print("🔄 Direction changed to ");
      Serial.println(dirCW ? "CW (clockwise)" : "CCW (counter-clockwise)");
      if (motorRunning) applyDirection();  // immediately update direction if spinning
    }
  }
  lastButtonState = reading;

  // --- RTC time update ---
  DateTime now = rtc.now();
  char buf[9];
  sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  Serial.print("Time: ");
  Serial.println(buf);

  // --- Start motor every minute ---
  if (now.second() == 0 && !motorRunning) {
    Serial.println("🕒 New minute → Motor ON for 30 s");
    startMotor();
    motorRunning = true;
    motorStartTime = millis();
  }

  // --- Stop after 30 s ---
  if (motorRunning && (millis() - motorStartTime >= RUN_DURATION)) {
    stopMotor();
    motorRunning = false;
    Serial.println("⏹️ Motor OFF\n");
  }

  delay(200);
}

// --- Helper functions ---
void startMotor() {
  applyDirection();
  analogWrite(ENA, pwmSpeed);
}

void stopMotor() {
  analogWrite(ENA, 0);
}

void applyDirection() {
  if (dirCW) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}
