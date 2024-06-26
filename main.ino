#include <TinyGPS++.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>
#include <RF24.h>
#include <WiFi.h>
#include <ESP32CAM.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 2     // DHT sensor connected to pin 2
#define DHTTYPE DHT22

TinyGPSPlus gps;
MPU6050 imu;
Servo servoMotor[4]; // 4 servos for aileron, elevator, rudder, throttle
RF24 radio(7, 8); // CE, CSN pins for NRF24L01
DHT dht(DHTPIN, DHTTYPE);

const int escPin = 9;
const int trigPin = 12;
const int echoPin = 11;
const int batteryPin = A0; // Monitoring battery level

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // GPS Serial
  Wire.begin();
  imu.initialize();
  
  // Attach servos to respective pins
  servoMotor[0].attach(10); // Aileron
  servoMotor[1].attach(11); // Elevator
  servoMotor[2].attach(12); // Rudder
  servoMotor[3].attach(escPin); // Throttle
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
  radio.startListening();
  
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  server.begin();
}

void loop() {
  readGPS();
  readIMU();
  readTemperatureHumidity();
  avoidObstacle();
  controlMotors();
  monitorBattery();
  handleClient();
}

void readGPS() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    }
  }
}

void readIMU() {
  int16_t ax, ay, az;
  imu.getAcceleration(&ax, &ay, &az);
  Serial.print("AX: "); Serial.print(ax);
  Serial.print(" AY: "); Serial.print(ay);
  Serial.print(" AZ: "); Serial.println(az);
}

void readTemperatureHumidity() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.println(" *C");
}

void avoidObstacle() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  
  if (distance < 50) {
    Serial.println("Obstacle detected");
    servoMotor[2].write(45); // Change rudder position
  }
}

void controlMotors() {
  if (radio.available()) {
    int throttle, aileron, elevator, rudder;
    radio.read(&throttle, sizeof(throttle));
    radio.read(&aileron, sizeof(aileron));
    radio.read(&elevator, sizeof(elevator));
    radio.read(&rudder, sizeof(rudder));
    
    servoMotor[0].write(aileron);
    servoMotor[1].write(elevator);
    servoMotor[2].write(rudder);
    analogWrite(escPin, throttle);
  }
}

void monitorBattery() {
  int batteryLevel = analogRead(batteryPin);
  float voltage = batteryLevel * (5.0 / 1023.0) * 3; // Adjust the multiplier based on your voltage divider
  Serial.print("Battery Voltage: ");
  Serial.println(voltage);
  if (voltage < 3.7) { // Low battery threshold
    Serial.println("Low battery! Returning to home.");
    returnToHome();
  }
}

void returnToHome() {
  // Logic to return to home
  // Use GPS coordinates to navigate back to the starting point
  // This is a simplified example and should be expanded with actual navigation logic
  Serial.println("Returning to home...");
}

void handleClient() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait for client to send data
  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Send response to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1>Surveillance RC Aircraft</h1>");
  client.println("<p>Real-time data and control interface</p>");
  client.println("</html>");
  delay(1);
}
