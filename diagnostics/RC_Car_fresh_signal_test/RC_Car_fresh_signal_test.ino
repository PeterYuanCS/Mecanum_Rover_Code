// Fresh, single-driver signal test.
// Confirmed physical wiring:
// L298N IN1 <- ESP32 IO1
// L298N IN2 <- ESP32 IO2
// L298N IN3 <- ESP32 IO41
// L298N IN4 <- ESP32 IO42
// L298N ENA and ENB are held HIGH by separate wires to the L298N 5V terminal.

const int IN1_PIN = 1;
const int IN2_PIN = 2;
const int IN3_PIN = 41;
const int IN4_PIN = 42;

void stopMotors() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  stopMotors();
  Serial.println("Fresh L298N signal test ready");
  Serial.println("IN1=IO1 IN2=IO2 IN3=IO41 IN4=IO42");
}

void loop() {
  Serial.println("A forward: OUT1/OUT2, 3 seconds");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(3000);
  stopMotors();
  delay(1500);

  Serial.println("B forward: OUT3/OUT4, 3 seconds");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(3000);
  stopMotors();
  delay(1500);
}
