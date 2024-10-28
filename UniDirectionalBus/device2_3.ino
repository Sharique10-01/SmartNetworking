#include <WiFi.h>
#include <esp_now.h>

#define STATUS_LED 2

struct Message {
    uint8_t initiator[6];
    char command[ESP_NOW_MAX_DATA_LEN];
};

// MAC address for Device 3
uint8_t nextDeviceMac[6] = {0xFC, 0xF5, 0xC4, 0x6E, 0x98, 0x4C}; // Device 3 MAC

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

    // Pass the message to Device 3
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
    Serial.println("Device 2 Ready");
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
    // Device 2 does not start communication; it only passes messages received
}
