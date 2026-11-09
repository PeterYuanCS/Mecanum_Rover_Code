// Direct ESP32-header test for the currently powered L298N.
// IN1 <- IO4, IN2 <- IO5, IN3 <- IO6, IN4 <- IO7.
// ENA and ENB are tied HIGH to the L298N 5V terminal.

const int IN1_PIN = 4;
const int IN2_PIN = 5;
const int IN3_PIN = 6;
const int IN4_PIN = 7;

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
  Serial.println("Direct two-motor L298N test ready");
}

void loop() {
  Serial.println("Both motors forward, 3 seconds");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(3000);

  Serial.println("Stop, 2 seconds");
  stopMotors();
  delay(2000);
}
