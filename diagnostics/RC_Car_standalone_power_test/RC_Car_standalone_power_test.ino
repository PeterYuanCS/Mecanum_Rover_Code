// Standalone-power diagnostic. Does not use the HC-SR04.
// Current rover orientation: former rear is the new forward.

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

void setup() {
  const int pins[] = {FRONT_IN1, FRONT_IN2, FRONT_IN3, FRONT_IN4,
                      REAR_IN1, REAR_IN2, REAR_IN3, REAR_IN4};
  for (int pin : pins) pinMode(pin, OUTPUT);
  stopAll();
}

void loop() {
  driveForward();
  delay(3000);
  stopAll();
  delay(2000);
}
