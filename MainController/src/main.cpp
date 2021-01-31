#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ethernet.h>
#include "../lib/U8g2/U8g2lib.h"
#include "../lib/DS18B20/DS18B20.h"
#include "../lib/OneWire/OneWire.h"
/**
 * Should be created before the first build,
 * content refers to the following:
 *
 * const char *ssid = "";       // Please fill in ssid of wifi
 * const char *password = "";   // Please fill in password of wifi
 *
 * IPAddress getIp0() {
 *     return IPAddress(<Your>, <LAN>, <IP>, <ADDRESS>);
 * }
 *
 * IPAddress getIp1() {
 *     return IPAddress(<Your>, <LAN>, <IP>, <ADDRESS>);
 * }
 *
 **/
#include "wifi_metadata.h"

#define SENSOR_PIN 0    // Pin marked as D3

const char *title_0 = "Sensor 0: ";
const char *title_1 = "Sensor 1: ";
const char *title_2 = "Sensor 2: ";

const IPAddress ip_0 = getIp0();
const IPAddress ip_1 = getIp1();

String *temp0 = nullptr;
String *temp1 = nullptr;
String *temp2 = nullptr;

// U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

DS18B20 sensor(new OneWire(SENSOR_PIN));

auto height = 0;
auto width = 0;

void setup() {
    Serial.begin(9600);
    Serial.print("Launching screen output...");
    u8g2.begin();
    Serial.println("Launched");

    Serial.print("Launching sensor...");
    u8g2.clearBuffer();
    u8g2.setFont(u8g_font_7x13);
    width = u8g2.getStrWidth(title_0);
    u8g2.setFont(u8g_font_10x20);
    height = u8g2.getFontAscent() * 1.2;
    u8g2.drawStr(0, u8g2.getFontAscent(), "Launching");
    u8g2.drawStr(0, 2.5 * u8g2.getFontAscent(), "sensor...");
    u8g2.sendBuffer();
    sensor.begin();

    Serial.println("Launched");
    u8g2.clearBuffer();
    u8g2.setFont(u8g_font_10x20);
    u8g2.drawStr(0, u8g2.getFontAscent(), "Launching");
    u8g2.drawStr(0, 2.5 * u8g2.getFontAscent(), "sensor...");
    u8g2.drawStr(0, 4 * u8g2.getFontAscent(), "Launched");
    u8g2.sendBuffer();

    Serial.print("Connecting to WiFi...");

    u8g2.clearBuffer();
    u8g2.setFont(u8g_font_10x20);
    u8g2.drawStr(0, u8g2.getFontAscent(), "Connecting");
    u8g2.drawStr(0, 2.5 * u8g2.getFontAscent(), "to Wifi...");
    u8g2.sendBuffer();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
    }

    Serial.println("Connected");
    u8g2.clearBuffer();
    u8g2.setFont(u8g_font_10x20);
    u8g2.drawStr(0, u8g2.getFontAscent(), "Connecting");
    u8g2.drawStr(0, 2.5 * u8g2.getFontAscent(), "to Wifi...");
    u8g2.drawStr(0, 4 * u8g2.getFontAscent(), "Connected");
    u8g2.sendBuffer();
}

void loop() {
    Serial.println("Connecting to sensor controller...");
    WiFiClient client;
    Serial.print("Connecting to ");
    Serial.print(ip_0);
    Serial.print("...");
    if (client.connect(ip_0, 80)) {
        Serial.println("Connected");

        delay(2000);

        temp0 = new String(client.readString());
        client.stop();
        if (temp0) {
            Serial.print("Temp 0: " + *temp0);
        }
    } else {
        Serial.println("Failed");
    }

    delay(2000);

    Serial.print("Connecting to ");
    client = WiFiClient();
    Serial.print(ip_1);
    Serial.print("...");
    if (client.connect(ip_1, 80)) {
        Serial.println("Connected");
        // data = emptyString;

        delay(2000);

        temp1 = new String(client.readString());
        client.stop();
        if (temp1) {
            Serial.print("Temp 1: " + *temp1);
        }
    } else {
        Serial.println("Failed");
    }

    Serial.print("Fetching data from sensor on board...");
    sensor.requestTemperatures();
    while (!sensor.isConversionComplete());
    Serial.println("fetched");
    temp2 = new String(sensor.getTempC());
    Serial.println(temp2->c_str());

    Serial.println("Drawing UI...");

    u8g2.clearBuffer();

    u8g2.setFont(u8g_font_7x13);
    u8g2.drawStr(0, u8g2.getFontAscent(), title_0);
    u8g2.setFont(u8g_font_10x20);
    u8g2.drawStr(width, u8g2.getFontAscent(), temp0 ? temp0->c_str() : "N/A");

    u8g2.setFont(u8g_font_7x13);
    u8g2.drawStr(0, height + u8g2.getFontAscent(), title_1);
    u8g2.setFont(u8g_font_10x20);
    u8g2.drawStr(width, height + u8g2.getFontAscent(), temp1 ? temp1->c_str() : "N/A");

    u8g2.setFont(u8g_font_7x13);
    u8g2.drawStr(0, height * 2 + u8g2.getFontAscent(), title_2);
    u8g2.setFont(u8g_font_10x20);
    u8g2.drawStr(width, height * 2 + u8g2.getFontAscent(), temp2 ? temp2->c_str() : "N/A");

    u8g2.sendBuffer();

    Serial.println("====================");
    Serial.println();
}