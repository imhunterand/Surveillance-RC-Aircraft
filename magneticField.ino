#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup() {
  Serial.begin(9600);
  if(!mag.begin()) {
    Serial.println("Could not find a valid HMC5883 sensor, check wiring!");
    while(1);
  }
}

void loop() {
  sensors_event_t event; 
  mag.getEvent(&event);
  Serial.print("Magnetic field X: "); Serial.println(event.magnetic.x);
  Serial.print("Magnetic field Y: "); Serial.println(event.magnetic.y);
  Serial.print("Magnetic field Z: "); Serial.println(event.magnetic.z);
  delay(1000);
}
