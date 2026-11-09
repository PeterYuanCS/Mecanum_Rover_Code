// HC-SR04 test at 3.3 V supply.
// Sensor: TRIG = IO40, ECHO = IO39.
// The battery holder stays OFF. This sketch holds the L298N control pins LOW.

const int TRIG_PIN = 40;
const int ECHO_PIN = 39;

const int motorPins[] = {17, 18, 15, 16, 42, 41, 6, 7};

void stopMotors() {
  for (int pin : motorPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

void setWheelsForward() {
  // Current rover orientation: former rear is the new forward.
  digitalWrite(17, HIGH); digitalWrite(18, LOW);
  digitalWrite(15, LOW);  digitalWrite(16, HIGH);
  digitalWrite(42, LOW);  digitalWrite(41, HIGH);
  digitalWrite(6, LOW);   digitalWrite(7, HIGH);
}

void driveMediumForTenSeconds() {
  // 60% software PWM. The 20 ms cycles make the rover roll smoothly.
  for (int elapsed = 0; elapsed < 10000; elapsed += 20) {
    setWheelsForward();
    delay(12);
    stopMotors();
    delay(8);
  }
  stopMotors();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  stopMotors();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.println("HC-SR04 3.3 V test: TRIG=IO40, ECHO=IO39");
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long echoMicroseconds = pulseIn(ECHO_PIN, HIGH, 30000);

  if (echoMicroseconds == 0) {
    Serial.println("No echo - 3.3 V may be too low for this HC-SR04.");
  } else {
    float distanceCm = echoMicroseconds * 0.0343f / 2.0f;
    Serial.print("Distance: ");
    Serial.print(distanceCm, 1);
    Serial.println(" cm");

    // Visual diagnostic when no readable USB serial console is available.
    if (distanceCm >= 3.0f && distanceCm <= 10.0f) {
      Serial.println("Obstacle close: drive forward for 10 seconds");
      driveMediumForTenSeconds();
      return;
    }
  }

  stopMotors();
  delay(50);
}
