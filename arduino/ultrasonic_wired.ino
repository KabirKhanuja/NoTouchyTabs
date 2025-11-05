#define TRIG_PIN 2
#define ECHO_PIN 3

unsigned long lastTriggerAt = 0;
const unsigned long COOLDOWN_MS = 1000;   // for ignoring further triggers for 1s
const float DELTA_THRESHOLD = 6.0;        // for sudden change required
const float ABS_MAX = 50.0;               // for ignoring the noise
const int SAMPLE_DELAY_MS = 80;           // for sampling interval

float prevDistance = -1.0;
unsigned long lastSampleTime = 0;

void setup() {
  Serial.begin(9600);
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
  if (duration == 0) return -1.0; // no reading
  return (duration * 0.0343) / 2.0;
}

void loop() {
  unsigned long now = millis();
  if (now - lastSampleTime < SAMPLE_DELAY_MS) return;
  lastSampleTime = now;

  float d = readDistanceCm();
  if (d <= 0) {
    return;
  }

  Serial.print("Distance: ");
  Serial.print(d);
  Serial.println(" cm");

  if (prevDistance < 0) {
    prevDistance = d;
    return;
  }

  float delta = prevDistance - d; // it'll be positive if object moved closer quickly

  // trigger conditions:
  // 1.distance decreased by DELTA_THRESHOLD (sudden approach)
  // 2.current distance less than ABS_MAX (to discard spurious very far readings)
  // 3.cooldown passed
  if (delta >= DELTA_THRESHOLD && d <= ABS_MAX && (now - lastTriggerAt) > COOLDOWN_MS) {
    Serial.println("SWITCH");
    lastTriggerAt = now;
  }

  // we'll update the prevDistance using light smoothing to avoid any kinda jitter
  prevDistance = (prevDistance * 0.7) + (d * 0.3);
}