// Reactive obstacle-avoidance rover.
// HC-SR04: VCC=3.3V, GND=GND, TRIG=IO40, ECHO=IO39.
// Current rover orientation: former rear is the new forward.

const int TRIG_PIN = 40;
const int ECHO_PIN = 39;
const float CLEARANCE_CM = 30.0f;

// Tune this after the first lifted-wheel turn test.
const unsigned long SCAN_TURN_MS = 350;

// Hardware PWM continues running independently of sensor measurements.
// All eight direction inputs need PWM because either input may drive a motor.
const unsigned long MOTOR_PWM_HZ = 1000;
const int MOTOR_PWM_BITS = 8;
const int FULL_DUTY = 255;
const int CRUISE_DUTY = 179;  // 70% electrical duty, not measured wheel speed.
const unsigned long CRUISE_PING_INTERVAL_MS = 70;
unsigned long lastCruisePingMs = 0;
bool cruisePingDue = true;

void motorWrite(int pin, int duty) {
  ledcWrite(pin, duty);
}

// Turns are deliberately gentler than cruise: 60% software PWM.
const unsigned long TURN_ON_MS = 12;
const unsigned long TURN_OFF_MS = 8;

const int FRONT_IN1 = 17;
const int FRONT_IN2 = 18;
const int FRONT_IN3 = 15;
const int FRONT_IN4 = 16;
const int REAR_IN1 = 42;
const int REAR_IN2 = 41;
const int REAR_IN3 = 6;
const int REAR_IN4 = 7;

void stopAll() {
  motorWrite(FRONT_IN1, 0); motorWrite(FRONT_IN2, 0);
  motorWrite(FRONT_IN3, 0); motorWrite(FRONT_IN4, 0);
  motorWrite(REAR_IN1, 0);  motorWrite(REAR_IN2, 0);
  motorWrite(REAR_IN3, 0);  motorWrite(REAR_IN4, 0);
}

void driveForward(int duty = FULL_DUTY) {
  // Clear opposing inputs first; leave active PWM running between readings.
  motorWrite(FRONT_IN2, 0); motorWrite(FRONT_IN3, 0);
  motorWrite(REAR_IN1, 0); motorWrite(REAR_IN3, 0);
  motorWrite(FRONT_IN1, duty); motorWrite(FRONT_IN4, duty);
  motorWrite(REAR_IN2, duty); motorWrite(REAR_IN4, duty);
}

void turnLeft() {
  // Left wheels reverse; right wheels drive forward.
  motorWrite(FRONT_IN2, 0); motorWrite(FRONT_IN4, 0);
  motorWrite(REAR_IN2, 0); motorWrite(REAR_IN3, 0);
  motorWrite(FRONT_IN1, FULL_DUTY); motorWrite(FRONT_IN3, FULL_DUTY);
  motorWrite(REAR_IN1, FULL_DUTY); motorWrite(REAR_IN4, FULL_DUTY);
}

void turnRight() {
  // Right wheels reverse; left wheels drive forward.
  motorWrite(FRONT_IN1, 0); motorWrite(FRONT_IN3, 0);
  motorWrite(REAR_IN1, 0); motorWrite(REAR_IN4, 0);
  motorWrite(FRONT_IN2, FULL_DUTY); motorWrite(FRONT_IN4, FULL_DUTY);
  motorWrite(REAR_IN2, FULL_DUTY); motorWrite(REAR_IN3, FULL_DUTY);
}

void turnLeftSoftFor(unsigned long durationMs) {
  for (unsigned long elapsed = 0; elapsed < durationMs;
       elapsed += TURN_ON_MS + TURN_OFF_MS) {
    turnLeft();
    delay(TURN_ON_MS);
    stopAll();
    delay(TURN_OFF_MS);
  }
  stopAll();
}

void turnRightSoftFor(unsigned long durationMs) {
  for (unsigned long elapsed = 0; elapsed < durationMs;
       elapsed += TURN_ON_MS + TURN_OFF_MS) {
    turnRight();
    delay(TURN_ON_MS);
    stopAll();
    delay(TURN_OFF_MS);
  }
  stopAll();
}

float measureOnceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoMicroseconds = pulseIn(ECHO_PIN, HIGH, 25000);
  if (echoMicroseconds == 0) return -1.0f;
  return echoMicroseconds * 0.0343f / 2.0f;
}

float measureDistanceCm() {
  float total = 0.0f;
  int validSamples = 0;

  for (int i = 0; i < 3; i++) {
    const float distanceCm = measureOnceCm();
    if (distanceCm > 0.0f) {
      total += distanceCm;
      validSamples++;
    }
    if (i < 2) delay(65);  // HC-SR04 needs >= 60 ms between pings.
  }

  return validSamples == 0 ? -1.0f : total / validSamples;
}

bool isClear(float distanceCm) {
  return distanceCm > CLEARANCE_CM;
}

void settleAndStop() {
  stopAll();
  delay(180);
}

void scanAndChooseDirection() {
  // Look left, return through centre, look right, then return to centre.
  turnLeftSoftFor(SCAN_TURN_MS);
  settleAndStop();
  const float leftCm = measureDistanceCm();

  turnRightSoftFor(SCAN_TURN_MS * 2);
  settleAndStop();
  const float rightCm = measureDistanceCm();

  turnLeftSoftFor(SCAN_TURN_MS);
  settleAndStop();

  const bool leftClear = isClear(leftCm);
  const bool rightClear = isClear(rightCm);

  if (!leftClear && !rightClear) {
    // No rear sensor is fitted, so do not reverse into unknown space.
    stopAll();
    delay(400);
    return;
  }

  if (leftClear && (!rightClear || leftCm >= rightCm)) {
    turnLeftSoftFor(SCAN_TURN_MS);
  } else {
    turnRightSoftFor(SCAN_TURN_MS);
  }
  settleAndStop();
}

void setup() {
  const int motorPins[] = {FRONT_IN1, FRONT_IN2, FRONT_IN3, FRONT_IN4,
                           REAR_IN1, REAR_IN2, REAR_IN3, REAR_IN4};
  for (int pin : motorPins) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }
  bool pwmReady = true;
  for (int pin : motorPins) {
    if (!ledcAttach(pin, MOTOR_PWM_HZ, MOTOR_PWM_BITS)) pwmReady = false;
  }
  stopAll();
  if (!pwmReady) {
    // Never drive with only some of the motor outputs configured.
    while (true) delay(1000);
  }

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  const unsigned long nowMs = millis();
  if (!cruisePingDue && nowMs - lastCruisePingMs < CRUISE_PING_INTERVAL_MS) {
    delay(1);
    return;
  }
  cruisePingDue = false;
  lastCruisePingMs = nowMs;
  // One current reading triggers the stop immediately; do not average a
  // close obstacle away with farther readings. Hardware PWM stays active
  // throughout pulseIn (bounded to 25 ms) and the wait between pings.
  const float forwardCm = measureOnceCm();

  // At 3.3 V this HC-SR04 may not return a far-distance echo. A real close
  // reading is an obstacle; no echo is treated as open space for cruising.
  if (forwardCm > 0.0f && forwardCm <= CLEARANCE_CM) {
    settleAndStop();
    scanAndChooseDirection();
    cruisePingDue = true;
    return;
  }

  driveForward(CRUISE_DUTY);
}
