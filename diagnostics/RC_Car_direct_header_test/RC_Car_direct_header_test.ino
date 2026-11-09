// Direct-header diagnostic: GPIO4 held HIGH, GPIO5 held LOW.
// Wire directly to the ESP32's own yellow header pins, not the expansion board.
// L298N: IN1 <- IO4, IN2 <- IO5, ENA <- L298N 5V.

const int IN1_TEST_PIN = 4;
const int IN2_TEST_PIN = 5;

void setup() {
  Serial.begin(115200);
  delay(1500);
  pinMode(IN1_TEST_PIN, OUTPUT);
  pinMode(IN2_TEST_PIN, OUTPUT);
  digitalWrite(IN1_TEST_PIN, HIGH);
  digitalWrite(IN2_TEST_PIN, LOW);
  Serial.println("Direct header test: IO4 HIGH, IO5 LOW");
}

void loop() {
  digitalWrite(IN1_TEST_PIN, HIGH);
  digitalWrite(IN2_TEST_PIN, LOW);
  delay(1000);
}
