#include <SoftwareSerial.h>

#define TRIG_PIN 2
#define ECHO_PIN 3
#define BT_RX 10 // Arduino pin that reads from HC-05 TX
#define BT_TX 11 // Arduino pin that writes to HC-05 RX

SoftwareSerial BTSerial(BT_RX, BT_TX); // RX, TX

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600); // HC-05 default
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30 ms
  if (duration == 0) return -1.0; // no reading
  float distance = (duration * 0.0343) / 2.0; // cm
  return distance;
}

void loop() {
  float d = readDistanceCm();
  if (d > 0) {
    Serial.print("Distance: ");
    Serial.print(d);
    Serial.println(" cm");

    if (d < 5.0) {
      BTSerial.println("SWITCH");
      Serial.println("SWITCH (sent to BT)");
      delay(1000); // simple debounce — wait 1s
    }
  }
  delay(100);
}