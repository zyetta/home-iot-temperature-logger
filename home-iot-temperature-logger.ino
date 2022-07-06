#include <PubSubClient.h>
#include <WiFi.h>

#include <string>

#include "mqtt_controller.h"
#include "secrets.h"
#include "wifi_controller.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 13
#define DHTTYPE DHT11

void reconnect();

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
float temp = 0;
uint64_t timeNow = millis();
const String mqtt_topic = "temperature/" + String(device_id);

void setup() {
    Serial.begin(115200);
    initWiFi();
    client.setServer(mqtt_broker, 1883);
    client.setCallback(mqttCallback);
    dht.begin();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    if (millis() - timeNow > 60000) {
        timeNow = millis();
        temp = dht.readTemperature();
        while(isnan(temp)) {
            temp = dht.readTemperature();
        }
        client.publish(mqtt_topic.c_str(), std::to_string(temp).c_str());
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
            Serial.println("Connected");
            client.publish("clients/temperature", device_id);
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
