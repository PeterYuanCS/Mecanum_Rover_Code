// Four-wheel forward diagnostic.
// Front L298N: IN1=IO17, IN2=IO18, IN3=IO15, IN4=IO16.
// Rear  L298N: IN1=IO42, IN2=IO41, IN3=IO6,  IN4=IO7.

const int FRONT_IN1 = 17;
const int FRONT_IN2 = 18;
const int FRONT_IN3 = 15;
const int FRONT_IN4 = 16;

const int REAR_IN1 = 42;
const int REAR_IN2 = 41;
const int REAR_IN3 = 6;
const int REAR_IN4 = 7;

void stopAll() {
  digitalWrite(FRONT_IN1, LOW);
  digitalWrite(FRONT_IN2, LOW);
  digitalWrite(FRONT_IN3, LOW);
  digitalWrite(FRONT_IN4, LOW);
  digitalWrite(REAR_IN1, LOW);
  digitalWrite(REAR_IN2, LOW);
  digitalWrite(REAR_IN3, LOW);
  digitalWrite(REAR_IN4, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  pinMode(FRONT_IN1, OUTPUT);
  pinMode(FRONT_IN2, OUTPUT);
  pinMode(FRONT_IN3, OUTPUT);
  pinMode(FRONT_IN4, OUTPUT);
  pinMode(REAR_IN1, OUTPUT);
  pinMode(REAR_IN2, OUTPUT);
  pinMode(REAR_IN3, OUTPUT);
  pinMode(REAR_IN4, OUTPUT);
  stopAll();
  Serial.println("Four-wheel-forward test ready");
}

void loop() {
  Serial.println("ALL FOUR WHEELS SLOW FORWARD for 5 seconds");

  // The L298N enable pins are tied to 5 V, so this uses gentle on/off
  // pulses to make the direction easy to inspect without rewiring.
  for (int pulse = 0; pulse < 17; pulse++) {
    // All four motor directions are reversed: the former rear is now forward.
    digitalWrite(FRONT_IN1, HIGH);
    digitalWrite(FRONT_IN2, LOW);
    digitalWrite(FRONT_IN3, LOW);
    digitalWrite(FRONT_IN4, HIGH);
    digitalWrite(REAR_IN1, LOW);
    digitalWrite(REAR_IN2, HIGH);
    digitalWrite(REAR_IN3, LOW);
    digitalWrite(REAR_IN4, HIGH);
    delay(75);
    stopAll();
    delay(225);
  }

  delay(2000);
}
