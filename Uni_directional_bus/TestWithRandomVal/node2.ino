#include <esp_now.h>
#include <WiFi.h>

#define NODE_2 2

// Define the data structure for receiving sensor data
typedef struct sensor_data_t {
    int id;       // ID of the sensor
    float value;  // Value from the sensor
} sensor_data_t;

// Callback function that runs when data is received
void onDataReceive(const esp_now_recv_info* info, const uint8_t *data, int len) {
    // Ensure we have received enough data
    if (len != sizeof(sensor_data_t)) {
        Serial.println("Received data size mismatch.");
        return;
    }

    // Cast received data to the structure
    sensor_data_t *receivedData = (sensor_data_t *)data;

    // Print received data
    Serial.printf("Node %d received ID: %d, Value: %.2f\n", NODE_2, receivedData->id, receivedData->value);

    // Forward to Node 3
    uint8_t node3Mac[] = { 0xEC, 0x64, 0xC9, 0x86, 0x61, 0xE8 };
    Serial.print("Forwarding to Node 3: ID: ");
    Serial.print(receivedData->id);
    Serial.print(", Value: ");
    Serial.println(receivedData->value);
    
    // Optional delay before sending to Node 3
    delay(100); // Delay for 100 milliseconds (adjust as necessary)

    // Send the received data to Node 3
    esp_err_t result = esp_now_send(node3Mac, (uint8_t*)receivedData, sizeof(sensor_data_t));
    if (result == ESP_OK) {
        Serial.println("Message forwarded successfully.");
    } else {
        Serial.printf("Error forwarding message: %d\n", result); // Print error code
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing ESP-NOW...");

    // Set device to be a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    // Disable Wi-Fi to only use ESP-NOW
    WiFi.disconnect();
    delay(100);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Add Node 3 as a peer
    uint8_t node3Mac[] = { 0xEC, 0x64, 0xC9, 0x86, 0x61, 0xE8 };
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, node3Mac, 6);
    peerInfo.channel = 0; // Use default channel
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add Node 3 as a peer.");
        return;
    } else {
        Serial.println("Node 3 added as a peer successfully.");
    }

    esp_now_register_recv_cb(onDataReceive);
}

void loop() {
    // No need to do anything in the loop
}
