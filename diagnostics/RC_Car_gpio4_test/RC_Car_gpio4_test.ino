// Single-pin diagnostic. After upload, GPIO4 is held HIGH continuously.
// With L298N ENA HIGH and IN2 tied to L298N GND, this should run OUT1/OUT2.

const int TEST_PIN = 4;

void setup() {
  Serial.begin(115200);
  delay(1500);
  pinMode(TEST_PIN, OUTPUT);
  digitalWrite(TEST_PIN, HIGH);
  Serial.println("GPIO4 HIGH continuously");
}

void loop() {
  digitalWrite(TEST_PIN, HIGH);
  delay(1000);
}
