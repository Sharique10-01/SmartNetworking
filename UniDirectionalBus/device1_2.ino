#include <WiFi.h>
#include <esp_now.h>

// Define built-in LED pin
#define STATUS_LED 2

// Define message structure
struct Message {
    uint8_t initiator[6];
    char command[ESP_NOW_MAX_DATA_LEN];
};

// MAC address for Device 2
uint8_t nextDeviceMac[6] = {0x08, 0xD1, 0xF9, 0xC8, 0xF3, 0xD4}; // Device 2 MAC

void formatMacAddress(const uint8_t *macAddr, char *buffer, int maxLength) {
    snprintf(buffer, maxLength, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    Message msg;
    memcpy(&msg, data, sizeof(Message));

    char initiatorStr[18];
    formatMacAddress(msg.initiator, initiatorStr, 18);
    
    Serial.printf("Received message from: %s - %s\n", initiatorStr, msg.command);

    if (strcmp("on", msg.command) == 0) {
        digitalWrite(STATUS_LED, HIGH);
    } else if (strcmp("off", msg.command) == 0) {
        digitalWrite(STATUS_LED, LOW);
    }

    // Pass the message to Device 2
    esp_now_send(nextDeviceMac, (uint8_t *)&msg, sizeof(msg));
}

void sentCallback(const uint8_t *macAddr, esp_now_send_status_t status) {
    char macStr[18];
    formatMacAddress(macAddr, macStr, 18);
    Serial.print("Last Packet Sent to: ");
    Serial.println(macStr);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.println("Device 1 Ready");
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    WiFi.disconnect();

    if (esp_now_init() == ESP_OK) {
        esp_now_register_recv_cb(receiveCallback);
        esp_now_register_send_cb(sentCallback);
    }

    pinMode(STATUS_LED, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        Message msg;
        memcpy(msg.initiator, WiFi.macAddress().c_str(), 6);
        strncpy(msg.command, input.c_str(), ESP_NOW_MAX_DATA_LEN);
        
        // Send the message to Device 2
        esp_now_send(nextDeviceMac, (uint8_t *)&msg, sizeof(msg));
    }
}
