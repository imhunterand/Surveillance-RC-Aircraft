const int pirPin = 7;
int pirState = LOW;

void setup() {
  pinMode(pirPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int val = digitalRead(pirPin);
  if (val == HIGH && pirState == LOW) {
    Serial.println("Motion detected!");
    pirState = HIGH;
  } else if (val == LOW && pirState == HIGH) {
    pirState = LOW;
  }
}
