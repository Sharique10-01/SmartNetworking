#include <WiFi.h> // For ESP32. Use <ESP8266WiFi.h> for ESP8266

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Wait for serial monitor to open
  while (!Serial) {}

  // Initialize WiFi (in station mode)
  WiFi.mode(WIFI_STA);  // Set to station mode

  // Adding a small delay to allow proper initialization
  delay(1000); // Wait for WiFi to initialize

  // Print ESP MAC address
  Serial.print("ESP MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Nothing to do in loop
}
