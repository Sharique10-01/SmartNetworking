#include <WiFi.h>
#include <esp_now.h>

// Define built-in LED pin (usually pin 2)
#define STATUS_LED 2

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength) {
    snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    char buffer[ESP_NOW_MAX_DATA_LEN + 1];
    int msgLen = min(ESP_NOW_MAX_DATA_LEN, dataLen);
    strncpy(buffer, (const char *)data, msgLen);
    buffer[msgLen] = 0;

    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    Serial.printf("Received message from: %s - %s\n", macStr, buffer);

    // Control the built-in LED based on received message
    if (strcmp("on", buffer) == 0) {
        digitalWrite(STATUS_LED, HIGH);  // Turn on LED
    } else if (strcmp("off", buffer) == 0) {
        digitalWrite(STATUS_LED, LOW);   // Turn off LED
    }

    // Print any random message to Serial Monitor
    if (strcmp("on", buffer) != 0 && strcmp("off", buffer) != 0) {
        Serial.printf("Broadcast message: %s\n", buffer);
    }
}

void sentCallback(const uint8_t *macAddr, esp_now_send_status_t status) {
    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    Serial.print("Last Packet Sent to: ");
    Serial.println(macStr);
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void broadcast(const String &message) {
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_now_peer_info_t peerInfo = {};
    memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
    if (!esp_now_is_peer_exist(broadcastAddress)) {
        esp_now_add_peer(&peerInfo);
    }
    esp_err_t result = esp_now_send(broadcastAddress, (const uint8_t *)message.c_str(), message.length());

    if (result == ESP_OK) {
        Serial.println("Broadcast message success");
    } else {
        Serial.println("Broadcast message failed");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    Serial.println("ESP-NOW Broadcast Demo");
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    WiFi.disconnect();

    if (esp_now_init() == ESP_OK) {
        Serial.println("ESP-NOW Init Success");
        esp_now_register_recv_cb(receiveCallback);
        esp_now_register_send_cb(sentCallback);
    } else {
        Serial.println("ESP-NOW Init Failed");
        delay(3000);
        ESP.restart();
    }

    pinMode(STATUS_LED, OUTPUT); // Set the built-in LED pin as output
}

void loop() {
    // Handle serial input
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove any extra whitespace
        broadcast(input); // Broadcast the received input
    }
}
