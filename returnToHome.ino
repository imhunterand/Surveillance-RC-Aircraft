#include <TinyGPS++.h>

TinyGPSPlus gps;

float homeLat, homeLng;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // GPS Serial
  homeLat = gps.location.lat();
  homeLng = gps.location.lng();
}

void loop() {
  returnToHome();
}

void returnToHome() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()) {
      float currentLat = gps.location.lat();
      float currentLng = gps.location.lng();
      
      if (abs(currentLat - homeLat) > 0.0001 || abs(currentLng - homeLng) > 0.0001) {
        // Logic to navigate back to home coordinates
        // Implement navigation logic here
        Serial.println("Returning to home...");
      } else {
        Serial.println("Arrived at home location.");
      }
    }
  }
}
