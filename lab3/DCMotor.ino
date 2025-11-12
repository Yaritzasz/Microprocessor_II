// Lab 3 - Step 2 (Your Pin Map)
// DC Motor Test with L293D
// Arduino: D5 -> L293D pin1 (EN1,2)
//           D4 -> L293D pin2 (IN1)
//           D3 -> L293D pin7 (IN2)

const int ENA = 5;  // PWM speed control
const int IN1 = 4;  // Direction control 1
const int IN2 = 3;  // Direction control 2

int pwmSpeed = 192; // 0–255 (≈3/4 speed)
bool isClockwise = true; // true = C, false = CC

// ----- RTC + LCD -----
#include <Wire.h>
#include <RTClib.h>              // Library for DS1307 RTC
#include <LiquidCrystal_I2C.h>   // Library for I2C 16x2 LCD

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows
// If your LCD address is different (0x3F, etc.), change 0x27.

// ----- Time variables (updated by timer interrupt) -----
volatile int curHour = 0;
volatile int curMin  = 0;
volatile int curSec  = 0;

// This flag tells loop() that 1 second passed
volatile bool oneSecondPassed = false;

// ---------- Helper: map PWM value to speed text ----------
const char* getSpeedLabel(int s) {
  if (s == 0)       return "0";      // stopped
  else if (s < 128) return "1/2";    // ~half speed
  else if (s < 220) return "3/4";    // ~3/4 speed
  else              return "Full";   // full speed
}

// ---------- Timer1 setup: interrupt every 1 second ----------
void setupTimer1() {
  noInterrupts();           // disable interrupts while we set it up

  TCCR1A = 0;               // clear control registers
  TCCR1B = 0;
  TCNT1  = 0;               // reset counter

  // Compare match value for 1 Hz:
  // 16 MHz / 1024 (prescaler) = 15625 counts/sec
  // We want interrupt every 1 sec -> OCR1A = 15624 (0..15624 = 15625 counts)
  OCR1A = 15624;

  // CTC mode (Clear Timer on Compare)
  TCCR1B |= (1 << WGM12);

  // Prescaler 1024: CS12 and CS10 bits set
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts();             // re-enable interrupts
}

// ---------- Timer1 interrupt: runs every 1 second ----------
ISR(TIMER1_COMPA_vect) {
  curSec++;
  if (curSec >= 60) {
    curSec = 0;
    curMin++;
    if (curMin >= 60) {
      curMin = 0;
      curHour++;
      if (curHour >= 24) {
        curHour = 0;
      }
    }
  }
  oneSecondPassed = true;   // tell loop() to refresh LCD
}

// ---------- Setup ----------
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lab 3 - DC Motor + RTC + LCD + Timer");

    // ----- RTC init -----
  if (!rtc.begin()) {
    Serial.println("RTC NOT found!");
    // we still continue, but time will be 00:00:00
  } else {
    // Read current time from RTC ONLY ONCE at startup
    DateTime now = rtc.now();
    curHour = now.hour();
    curMin  = now.minute();
    curSec  = now.second();
  }

  // ----- LCD init -----
  lcd.init();       // initialize LCD
  lcd.backlight();  // turn on backlight

  // First message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lab 3 Starting");
  lcd.setCursor(0, 1);
  lcd.print("Please wait...");

  delay(1000);

  // ----- Timer interrupt -----
  setupTimer1();
}

// ---------- Update LCD display ----------
void updateLCD() {
  // Copy volatile time safely
  noInterrupts();
  int h = curHour;
  int m = curMin;
  int s = curSec;
  bool dir = isClockwise;
  interrupts();

  lcd.clear();

 // Row 0: HH:MM:SS
  lcd.setCursor(0, 0);
  if (h < 10) lcd.print('0');
  lcd.print(h);
  lcd.print(':');
  if (m < 10) lcd.print('0');
  lcd.print(m);
  lcd.print(':');
  if (s < 10) lcd.print('0');
  lcd.print(s);


  // Row 1: direction + speed
  lcd.setCursor(0, 1);
  lcd.print("D:");
  if (dir) {
    lcd.print("C ");     // clockwise -> "C"
  } else {
    lcd.print("CC");     // counter-clockwise -> "CC"
  }

  lcd.print(" S:");
  lcd.print(getSpeedLabel(pwmSpeed));
}

// ---------- Loop ----------
// For now we keep your simple forward/stop/reverse test.
// The timer interrupt keeps counting time in the background.
void loop() {

  // If 1 second passed, refresh LCD
  if (oneSecondPassed) {
    oneSecondPassed = false;
    updateLCD();
  }

  // Forward rotation
  Serial.println("Motor: FORWARD");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwmSpeed);
  delay(5000);

    // Check LCD flag during delays too
  if (oneSecondPassed) {
    oneSecondPassed = false;
    updateLCD();
  }

  // Stop
  Serial.println("Motor: STOP");
  analogWrite(ENA, 0);
  pwmSpeed = 0;                    // for LCD label
  updateLCD();
  delay(1000);

  // Reverse rotation
  Serial.println("Motor: REVERSE");
  isClockwise = false;             // for LCD
  pwmSpeed = 192;                  // back to 3/4 speed
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwmSpeed);
  delay(5000);

   if (oneSecondPassed) {
    oneSecondPassed = false;
    updateLCD();
  }

  // Stop again
  Serial.println("Motor: STOP");
  analogWrite(ENA, 0);
  pwmSpeed = 0;
  updateLCD();
  delay(1000);
}
