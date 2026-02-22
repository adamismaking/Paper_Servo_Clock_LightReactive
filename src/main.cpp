#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>

/**
 * Photoresistor & Servo Control with LCD Display
 * Displays light level and servo angle on a 16x2 LCD.
 * Fixes servo buzz by detaching it after a period of inactivity.
 */

// --- Pin Definitions ---
const int SENSOR_PIN = A0;
const int SERVO_PIN = 9;
const int LED_PIN = 13;

// --- Component Objects ---
Servo myServo;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

// --- Smoothing & Movement Variables ---
float smoothedValue = 500; // Initial guess for sensor reading
const float ALPHA = 0.05;   // Smoothing factor (lower is smoother)
float currentServoAngle = 90.0; // Use float for precision
const float SERVO_SPEED = 0.1;  // Movement speed (0.01 = slow, 1.0 = instant)

// --- Buzz-Fix Variables ---
const int DEADBAND = 2; // Ignore angle changes smaller than this
unsigned long lastMoveTime = 0; // Timestamp of the last servo movement
const unsigned long IDLE_TIMEOUT_MS = 1000; // Detach after 1 second of inactivity

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Light-Servo Ctrl");
  
  // Initialize sensor/servo with a real reading to avoid a jump at the start
  int initialRead = analogRead(SENSOR_PIN);
  smoothedValue = initialRead;
  currentServoAngle = map(initialRead, 0, 1023, 0, 180);
  
  Serial.println("--- Light-Servo Control with LCD Initialized ---");
  // Servo is not attached at start to prevent initial jump/buzz
}

void loop() {
  // 1. Smooth the sensor input
  int rawValue = analogRead(SENSOR_PIN);
  smoothedValue = (ALPHA * rawValue) + ((1.0 - ALPHA) * smoothedValue);
  
  // 2. Map smoothed value to a target angle
  float targetAngle = map(smoothedValue, 0, 1023, -10, 190); // Wider map to help some servos
  targetAngle = constrain(targetAngle, 0, 180);
  
  // 3. Check if the change is significant enough to warrant a move
  if (abs(targetAngle - currentServoAngle) > DEADBAND) {
    // If servo is detached, re-attach it before moving
    if (!myServo.attached()) {
      Serial.println("Re-attaching servo...");
      myServo.attach(SERVO_PIN);
      delay(10); // Give it a moment to initialize
    }
    
    // 4. Smoothly interpolate towards the target angle
    currentServoAngle += (targetAngle - currentServoAngle) * SERVO_SPEED;
    myServo.write((int)currentServoAngle);
    
    // Update the timestamp of the last movement
    lastMoveTime = millis();

  } else {
    // 5. If the servo has been idle for long enough, detach it
    if (myServo.attached() && (millis() - lastMoveTime > IDLE_TIMEOUT_MS)) {
      Serial.println("Detaching servo due to inactivity.");
      myServo.detach();
    }
  }
  
  // 6. Update debugging info and LCD (throttled to prevent flicker)
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 150) { // Update rate for LCD and Serial
    // -- Serial Debugging --
    Serial.print("Target: ");
    Serial.print((int)targetAngle);
    Serial.print(" | Actual: ");
    Serial.print((int)currentServoAngle);
    Serial.print(" | Attached: ");
    Serial.println(myServo.attached() ? "Yes" : "No");
    
    // -- LCD Update --
    // Row 0: Light Level
    lcd.setCursor(0, 0);
    lcd.print("Light: ");
    lcd.print((int)smoothedValue);
    lcd.print("    "); // Pad with spaces to clear old characters

    // Row 1: Servo Angle
    lcd.setCursor(0, 1);
    lcd.print("Servo: ");
    lcd.print((int)currentServoAngle);
    lcd.print(" deg");
    lcd.print("   "); // Pad with spaces
    
    lastPrint = millis();
  }

  // Visual indicator for darkness
  digitalWrite(LED_PIN, (smoothedValue < 400) ? HIGH : LOW);

  delay(20); // Main loop delay
}
