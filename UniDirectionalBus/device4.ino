#include <WiFi.h>
#include <esp_now.h>

#define STATUS_LED 2

struct Message {
    uint8_t initiator[6];
    char command[ESP_NOW_MAX_DATA_LEN];
};

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
        digitalWrite(
