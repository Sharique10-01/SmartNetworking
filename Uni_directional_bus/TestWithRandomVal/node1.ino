#include <WiFi.h>
#include <esp_now.h>

typedef struct sensor_data_t {
    int id;
    float value;
} sensor_data_t;

sensor_data_t myData;

// MAC address of Node 2
uint8_t peer_addr_B[] = { 0xFC, 0xF5, 0xC4, 0x6E, 0x97, 0x6C }; // Replaced with  MAC of Node 2

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Add Node 2 as a peer
    
    esp_now_peer_info_t peer_B;
    memcpy(peer_B.peer_addr, peer_addr_B, 6);
    peer_B.channel = 0;  // Use the current channel
    peer_B.encrypt = false;

    // Add the peer
    if (esp_now_add_peer(&peer_B) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    myData.id = 1; // Set ID for this node this will be unique and random for every new msg
}

void loop() {
    // Update data value
    myData.value = random(1, 100) / 10.0; // Random value for demonstration
    Serial.println("Sending message: Hello from Node 1");
    
    // Send data to Node 2
    esp_err_t result = esp_now_send(peer_addr_B, (uint8_t *)&myData, sizeof(myData));
    
    if (result == ESP_OK) {
        Serial.println("Message sent successfully");
    } else {
        Serial.printf("Error sending message: %s\n", esp_err_to_name(result));
    }

    delay(5000); // Wait before sending the next message
}
