#include <SoftwareSerial.h>
#define TRIG_PIN 2
#define ECHO_PIN 3
#define BT_RX 10
#define BT_TX 11
SoftwareSerial BTSerial(BT_RX, BT_TX);

unsigned long lastTriggerAt = 0;
const unsigned long COOLDOWN_MS = 1000;
const float DELTA_THRESHOLD = 6.0;
const float ABS_MAX = 50.0;
const int SAMPLE_DELAY_MS = 80;

float prevDistance = -1.0;
unsigned long lastSampleTime = 0;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  delay(200);
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
  unsigned long now = millis();
  if (now - lastSampleTime < SAMPLE_DELAY_MS) return;
  lastSampleTime = now;

  float d = readDistanceCm();
  if (d <= 0) return;

  Serial.print("Distance: ");
  Serial.print(d);
  Serial.println(" cm");

  if (prevDistance < 0) {
    prevDistance = d;
    return;
  }

  float delta = prevDistance - d;
  if (delta >= DELTA_THRESHOLD && d <= ABS_MAX && (now - lastTriggerAt) > COOLDOWN_MS) {
    BTSerial.println("SWITCH");
    Serial.println("SWITCH (sent to BT)");
    lastTriggerAt = now;
  }

  prevDistance = (prevDistance * 0.7) + (d * 0.3);
}