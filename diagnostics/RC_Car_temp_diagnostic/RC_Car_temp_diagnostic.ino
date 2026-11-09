// Temporary L298N diagnostic for the driver wired as follows:
// IN1 -> ESP32-S3 IO1, IN2 -> IO2, IN3 -> IO41, IN4 -> IO42.
// ENA and ENB must already be tied HIGH on the L298N.

const int IN1_PIN = 1;
const int IN2_PIN = 2;
const int IN3_PIN = 41;
const int IN4_PIN = 42;

void stopAll() {
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
  stopAll();

  Serial.println("L298N temporary diagnostic ready");
  Serial.println("Mapping: IN1=IO1 IN2=IO2 IN3=IO41 IN4=IO42");
}

void loop() {
  Serial.println("TEST A: OUT1/OUT2 forward for 3 seconds");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(3000);
  stopAll();
  Serial.println("STOP for 2 seconds");
  delay(2000);

  Serial.println("TEST B: OUT3/OUT4 forward for 3 seconds");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(3000);
  stopAll();
  Serial.println("STOP for 2 seconds");
  delay(2000);
}
