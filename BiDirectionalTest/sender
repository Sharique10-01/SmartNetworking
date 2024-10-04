#include <esp_now.h>
#include <WiFi.h>

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a structured object
struct_message myData, receivedData;

// Peer info
esp_now_peer_info_t peerInfo;

// MAC Address of the other ESP32 (Receiver)
uint8_t peerAddress[] = {0x08, 0x6F, 0xD1, 0xC8, 0xF3, 0xD4}; // Update with receiver's MAC address

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function for when data is received
void OnDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Received Response: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(receivedData.a);
  Serial.print("Integer Value: ");
  Serial.println(receivedData.b);
  Serial.print("Float Value: ");
  Serial.println(receivedData.c);
  Serial.print("Boolean Value: ");
  Serial.println(receivedData.d);
  Serial.println();
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send and receive callback
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Prepare data to send
  strcpy(myData.a, "TEST 1 !");
  myData.b = random(1, 20);
  myData.c = myData.b * 1.3;
  myData.d = !myData.d;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(peerAddress, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }

  delay(2000);  // Wait before sending again
}
