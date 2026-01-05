#define BLYNK_TEMPLATE_ID "TMPL3KNCxq2ut"
#define BLYNK_TEMPLATE_NAME "pipe inspection robot"
#define BLYNK_AUTH_TOKEN "l0C-gIQa7AUePdgGYoa75YRTI5MxxvSN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h" 

// WiFi credentials
char ssid[] = "BSNL-b59310";
char pass[] = "muha6175";

// --- NEW MOTOR CONTROL PINS ---

#define IN1 32 // Motor A (Left) - Input 1
#define IN2 33 // Motor A (Left) - Input 2
#define IN3 18 // Motor B (Right) - Input 1
#define IN4 19 // Motor B (Right) - Input 2
#define ENA 25 // Enable pin for Motor A (Left)
#define ENB 26 // Enable pin for Motor B (Right)

#define MQ4_PIN 34  
// DHT Sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic sensor pins
#define TRIG_PIN 13
#define ECHO_PIN 12

// Buzzer
#define BUZZER_PIN 2

// Variables for Ultrasonic sensor
long duration;
float distance;
bool obstacleDetected = false; // Flag to prevent repeated log events

BlynkTimer timer; // Blynk timer object

// --- NEW MOTOR CONTROL FUNCTIONS ---
// These functions set the digital states for the motor driver input pins.
// ENA and ENB will be HIGH in setup for constant power.

// Moves both motors forward
void forward() {
  Serial.println("Moving Forward");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Moves both motors backward
void backward() {
  Serial.println("Moving Backward");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Turns the robot left (e.g., left motor backward, right motor forward)
void left() {
  Serial.println("Turning Left");
  digitalWrite(IN1, LOW);  // Motor A (Left) backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); // Motor B (Right) forward
  digitalWrite(IN4, LOW);
}

// Turns the robot right (e.g., left motor forward, right motor backward)
void right() {
  Serial.println("Turning Right");
  digitalWrite(IN1, HIGH); // Motor A (Left) forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  // Motor B (Right) backward
  digitalWrite(IN4, HIGH);
}

// Stops both motors by setting all input pins to LOW
void stopMotors() {
  Serial.println("Stopping Motors");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// --- Blynk BLYNK_WRITE functions for motor control ---
// These functions are triggered when the corresponding Blynk button widget is pressed or released.


// Virtual Pin V0 for Forward movement
BLYNK_WRITE(V0) {
  if (param.asInt()) { // Button pressed (value is 1)
    forward();
  } else { // Button released (value is 0)
    stopMotors();
  }
}

// Virtual Pin V1 for Backward movement
BLYNK_WRITE(V1) {
  if (param.asInt()) { // Button pressed (value is 1)
    backward();
  } else { // Button released (value is 0)
    stopMotors();
  }
}

// Virtual Pin V2 for Left turn
BLYNK_WRITE(V2) {
  if (param.asInt()) { // Button pressed (value is 1)
    left();
  } else { // Button released (value is 0)
    stopMotors();
  }
}

// Virtual Pin V3 for Right turn
BLYNK_WRITE(V3) {
  if (param.asInt()) { // Button pressed (value is 1)
    right();
  } else { // Button released (value is 0)
    stopMotors();
  }
}



// Reads DHT sensor data (Temperature & Humidity) and sends to Blynk
void sendDHTData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" °C | Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  Blynk.virtualWrite(V4, t); // Temperature to V4
  Blynk.virtualWrite(V5, h); // Humidity to V5
}

// Reads distance from Ultrasonic sensor and triggers buzzer/Blynk event if obstacle is close
void detectObstacle() {
  // Clear the TRIG_PIN by setting it LOW for a moment
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Set the TRIG_PIN HIGH for 10 microseconds to send a pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the pulse on the ECHO_PIN
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculate distance in cm (speed of sound = 0.034 cm/microsecond)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Blynk.virtualWrite(V6, (int)distance); // Send distance to Blynk Virtual Pin V6
  Serial.println("Sent distance to Blynk V6");

  // Check for obstacle and activate buzzer
  if (distance > 0 && distance < 15) { // Obstacle within 15 cm (adjust as needed)
    digitalWrite(BUZZER_PIN, HIGH); // Turn ON buzzer

    // Send Blynk event only once per obstacle detection
    if (!obstacleDetected) {
      Blynk.logEvent("obstacle_detected", "Obstacle Detected!");
      obstacleDetected = true;
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Turn OFF buzzer
    obstacleDetected = false; // Reset flag when obstacle is no longer detected
  }
}

// Reads MQ4 gas sensor value and sends to Blynk, triggers event if high
void checkGas() {
  int gasValue = analogRead(MQ4_PIN); // Read analog value from MQ4 sensor
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);

  Blynk.virtualWrite(V7, gasValue); // Show gas value on Blynk gauge (V7)

  
  if (gasValue > 1000) {
    Blynk.virtualWrite(V8, 255); // Turn ON Blynk LED (V8)
    Blynk.logEvent("gas_leak", "🚨 Gas Leakage Detected!"); // Trigger Blynk event
  } else {
    Blynk.virtualWrite(V8, 0);   // Turn OFF Blynk LED
  }
}

void setup() {
  Serial.begin(115200); // Initialize Serial communication for debugging

  // --- NEW Motor pin initialization ---
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // --- Enable both motors constantly (no PWM) ---
  digitalWrite(ENA, HIGH); // Set ENA to HIGH for full speed
  digitalWrite(ENB, HIGH); // Set ENB to HIGH for full speed

  stopMotors(); // Ensure motors are stopped initially

  // --- Sensor and Buzzer pin initialization (No changes here) ---
  dht.begin(); // Initialize DHT sensor

  pinMode(TRIG_PIN, OUTPUT); // Ultrasonic sensor Trigger pin
  pinMode(ECHO_PIN, INPUT);  // Ultrasonic sensor Echo pin

  pinMode(BUZZER_PIN, OUTPUT); // Buzzer pin
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially

  // --- Connect to WiFi and Blynk ---
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // --- Set up Blynk timers for periodic sensor readings ---
  timer.setInterval(2000L, sendDHTData);    // Read DHT data every 2 seconds
  timer.setInterval(500L, detectObstacle);  // Check obstacle every 0.5 seconds
  timer.setInterval(1000L, checkGas);      // Check gas sensor every 1 second
}

void loop() {
  Blynk.run(); // Keep Blynk connected and process commands
  timer.run(); // Run Blynk timers for sensor readings
}