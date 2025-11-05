#define TRIG_PIN 2
#define ECHO_PIN 3

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  if (duration == 0) return -1.0;
  return (duration * 0.0343) / 2.0;
}

void loop() {
  float d = readDistanceCm();
  if (d > 0) {
    Serial.print("Distance: ");
    Serial.print(d);
    Serial.println(" cm");

    if (d < 5.0) {
      Serial.println("SWITCH");
      delay(1000);
    }
  }
  delay(100);
}