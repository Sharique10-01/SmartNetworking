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
struct_message myData, myResponseData;

// Peer info
esp_now_peer_info_t peerInfo;

// MAC Address of the other ESP32 (Sender/Responder)
uint8_t peerAddress[] = {0xFC, 0xF5, 0xC4, 0x6E, 0x97, 0x6C}; // Update with sender's MAC address

// Callback function executed when data is received
void OnDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Integer Value: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
  Serial.print("Boolean Value: ");
  Serial.println(myData.d);
  Serial.println();

  // Prepare and send response
  strcpy(myResponseData.a, "Received ACK!");
  myResponseData.b = myData.b + 1;  // Just modify data to demonstrate response
  myResponseData.c = myData.c + 1.5;
  myResponseData.d = !myData.d;

  esp_err_t result = esp_now_send(mac_addr, (uint8_t *) &myResponseData, sizeof(myResponseData));
  if (result == ESP_OK) {
    Serial.println("Response sent");
  } else {
    Serial.println("Error sending response");
  }
}

// Callback function for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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
  // Nothing to do here, just waiting for data
}
