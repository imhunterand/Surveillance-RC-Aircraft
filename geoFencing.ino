#include <TinyGPS++.h>

TinyGPSPlus gps;

float minLat, maxLat, minLng, maxLng;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // GPS Serial
  
  // Set geo-fence boundaries
  minLat = -6.200000;
  maxLat = -6.100000;
  minLng = 106.800000;
  maxLng = 106.900000;
}

void loop() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()) {
      float currentLat = gps.location.lat();
      float currentLng = gps.location.lng();
      
      if (currentLat < minLat || currentLat > maxLat || currentLng < minLng || currentLng > maxLng) {
        Serial.println("Out of bounds! Returning to safe zone.");
        // Implement logic to return to safe zone
      }
    }
  }
}
