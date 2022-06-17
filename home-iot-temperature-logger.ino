#include <PubSubClient.h>
#include <WiFi.h>

#include <string>

#include "mqtt_controller.h"
#include "secrets.h"
#include "wifi_controller.h"

void reconnect();

WiFiClient espClient;
PubSubClient client(espClient);

uint64_t timeNow = millis();
const String mqtt_topic = "temperature/" + String(device_id);

void setup() {
    Serial.begin(115200);
    initWiFi();
    client.setServer(mqtt_broker, 1883);
    client.setCallback(mqttCallback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    if (millis() - timeNow > 60000) {
        timeNow = millis();
        Serial.println("Sampling Data");

        client.publish(mqtt_topic.c_str(),
                       std::to_string(random(10, 30)).c_str());
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
