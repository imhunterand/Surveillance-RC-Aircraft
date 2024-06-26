const int airQualityPin = A1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int airQuality = analogRead(airQualityPin);
  Serial.print("Air Quality Index: ");
  Serial.println(airQuality);
  delay(1000);
}
