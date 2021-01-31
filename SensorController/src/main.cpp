#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../lib/OneWire/OneWire.h"
#include "../lib/DS18B20/DS18B20.h"
#include "wifi_metadata.h"

WiFiServer server(80);

OneWire oneWire(2);
DS18B20 sensor(&oneWire);

void setup() {
    Serial.begin(9600);
    sensor.begin();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.println("Connecting...");
    }
    Serial.println("Connected!");
    server.begin();
}

void loop() {
    Serial.println("Waiting for client...");
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    Serial.println("Client available!");
    Serial.println("Getting temperature...");
    sensor.requestTemperatures();
    while (!sensor.isConversionComplete());
    auto str = String(sensor.getTempC());
    Serial.println("Temperature: " + str);
    client.println(str.c_str());
    client.flush();
    delay(500);
    client.stop();
    Serial.println("Client closed!");
}