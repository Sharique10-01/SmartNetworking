#include <esp_now.h>
#include <WiFi.h>

typedef struct sensor_data_t {
    int id;
    float value;
} sensor_data_t;

void onDataReceived(const esp_now_recv_info* info, const uint8_t *data, int len) {
    sensor_data_t receivedData;
    memcpy(&receivedData, data, sizeof(receivedData));
    Serial.printf("Received data from %s: id=%d, value=%.2f\n", macToString(info->des_addr).c_str(), receivedData.id, receivedData.value);
}

String macToString(const uint8_t *mac) {
    String macStr;
    for (int i = 0; i < 6; i++) {
        if (i > 0) macStr += ":";
        macStr += String(mac[i], HEX);
    }
    return macStr;
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    delay(500);
    
    // Print MAC Address
    uint8_t mac[6];
    WiFi.macAddress(mac);  // Get MAC address directly to byte array
    Serial.print("Node 3 MAC Address: ");
    Serial.println(macToString(mac));

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the receive callback
    esp_now_register_recv_cb(onDataReceived);
    Serial.println("Node 3 initialized and waiting for messages.");
}

void loop() {
    // No operation in loop; waiting for messages
}
