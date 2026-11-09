// Rover obstacle-stop program.
// HC-SR04: VCC=3.3V, GND=GND, TRIG=IO40, ECHO=IO39.
// Current rover orientation: former rear is the new forward.

const int TRIG_PIN = 40;
const int ECHO_PIN = 39;
const float STOP_DISTANCE_CM = 30.0f;

const int FRONT_IN1 = 17;
const int FRONT_IN2 = 18;
const int FRONT_IN3 = 15;
const int FRONT_IN4 = 16;
const int REAR_IN1 = 42;
const int REAR_IN2 = 41;
const int REAR_IN3 = 6;
const int REAR_IN4 = 7;

void stopAll() {
  digitalWrite(FRONT_IN1, LOW); digitalWrite(FRONT_IN2, LOW);
  digitalWrite(FRONT_IN3, LOW); digitalWrite(FRONT_IN4, LOW);
  digitalWrite(REAR_IN1, LOW);  digitalWrite(REAR_IN2, LOW);
  digitalWrite(REAR_IN3, LOW);  digitalWrite(REAR_IN4, LOW);
}

void driveForward() {
  digitalWrite(FRONT_IN1, HIGH); digitalWrite(FRONT_IN2, LOW);
  digitalWrite(FRONT_IN3, LOW);  digitalWrite(FRONT_IN4, HIGH);
  digitalWrite(REAR_IN1, LOW);   digitalWrite(REAR_IN2, HIGH);
  digitalWrite(REAR_IN3, LOW);   digitalWrite(REAR_IN4, HIGH);
}

float measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long echoMicroseconds = pulseIn(ECHO_PIN, HIGH, 25000);
  if (echoMicroseconds == 0) return -1.0f;
  return echoMicroseconds * 0.0343f / 2.0f;
}

void setup() {
  const int motorPins[] = {FRONT_IN1, FRONT_IN2, FRONT_IN3, FRONT_IN4,
                           REAR_IN1, REAR_IN2, REAR_IN3, REAR_IN4};
  for (int pin : motorPins) pinMode(pin, OUTPUT);
  stopAll();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  float distanceCm = measureDistanceCm();

  // No echo is treated as unsafe: stay stopped.
  if (distanceCm < 0 || distanceCm <= STOP_DISTANCE_CM) {
    stopAll();
  } else {
    driveForward();
  }

  // HC-SR04 needs at least 60 ms between measurements.
  delay(70);
}
