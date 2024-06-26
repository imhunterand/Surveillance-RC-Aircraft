#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}

void loop() {
  float altitude = bmp.readAltitude();
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" meters");
  delay(1000);
}
