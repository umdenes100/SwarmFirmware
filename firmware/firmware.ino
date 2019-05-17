#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define WIFI_NETWORK "VisionSystem1116-2.4"

WiFiUDP client;

void setup() {
    // Begin serial communication with Arduino
    Serial.begin(9600);

    // Connect to WiFi network
    WiFi.begin("4308", "Alcohole");
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        ESP.restart();
    }

    // Begin listening on port 8080
    client.begin(8080);
}

void loop() {
    Serial.println(WiFi.localIP());
    delay(400);
    // If there is an outgoing message...
    if (Serial.available() >= 5) {
        // Read in message from Arduino
        byte buffer[5];
        for (int i = 0; i < 5; i++) {
            buffer[i] = Serial.read();
        }

        client.beginPacket(IPAddress(buffer), 8080);
        client.write(buffer[4]);
        client.endPacket();
    }

    // If there is an incoming message
    int packetSize = client.parsePacket();
    if (packetSize) {
        // Attach IP address as first 4 bytes
        byte buffer[5];
        IPAddress remoteIp = client.remoteIP();
        for (int i = 0; i < 4; i++) {
            buffer[i] = remoteIp[i];
        }

        // Append payload
        buffer[4] = client.read();
        // Send to Arduino
        Serial.write(buffer, 5);
    }
}
