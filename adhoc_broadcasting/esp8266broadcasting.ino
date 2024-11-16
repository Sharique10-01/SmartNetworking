#include <ESP8266WiFi.h>
#include <espnow.h>

// Define built-in LED pin (usually pin 2 for ESP8266)
#define STATUS_LED 2

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength) {
    snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

// Updated receive callback function
void receiveCallback(uint8_t *macAddr, uint8_t *data, uint8_t dataLen) {
    char buffer[250]; // Adjust buffer size if necessary
    int msgLen = min(250, (int)dataLen); // Cast dataLen to int to resolve type conflict
    strncpy(buffer, (char *)data, msgLen);
    buffer[msgLen] = 0;

    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    Serial.printf("Received message from: %s - %s\n", macStr, buffer);

    // Corrected logic for the LED control
    if (strcmp("on", buffer) == 0) {
        digitalWrite(STATUS_LED, LOW);  // Turn LED on (LOW might turn on the LED)
        Serial.println("Turning LED ON");
    } else if (strcmp("off", buffer) == 0) {
        digitalWrite(STATUS_LED, HIGH);   // Turn LED off (HIGH might turn off the LED)
        Serial.println("Turning LED OFF");
    } else {
        Serial.printf("Broadcast message: %s\n", buffer);
    }
}

// Updated sent callback function for ESP8266
void sentCallback(uint8_t *macAddr, uint8_t status) {
    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    Serial.print("Last Packet Sent to: ");
    Serial.println(macStr);
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");  // For ESP8266, 0 indicates success
}

void broadcast(const String &message) {
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // ESP-NOW Broadcast address
    
    // Ensure the peer is added only once
    if (!esp_now_is_peer_exist(broadcastAddress)) {
        esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    }
    
    // Send broadcast message
    esp_now_send(broadcastAddress, (uint8_t *)message.c_str(), message.length());
    Serial.println("Broadcast message sent.");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    Serial.println("ESP-NOW Broadcast Demo");
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    WiFi.disconnect();

    if (esp_now_init() == 0) {
        Serial.println("ESP-NOW Init Success");
        esp_now_register_recv_cb(receiveCallback);  // Register receive callback
        esp_now_register_send_cb(sentCallback);     // Register send callback
    } else {
        Serial.println("ESP-NOW Init Failed");
        delay(3000);
        ESP.restart();
    }

    pinMode(STATUS_LED, OUTPUT); 
    digitalWrite(STATUS_LED, HIGH);  // Set the LED to off state initially
}

void loop() {
    // Handle serial input for broadcasting
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove extra whitespace
        broadcast(input);  // Broadcast the received input
    }
}
