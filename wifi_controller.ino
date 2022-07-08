#include "secrets.h"
#include "wifi_controller.h"

void initWiFi() {
    uint64_t timeNow = millis();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED ) {
        Serial.print('.');
        delay(1000);
        if(millis() - timeNow > 10000) {
            timeNow = millis();
            WiFi.disconnect();
            WiFi.reconnect();
            Serial.print("\nRetrying");

        }
    }
    Serial.println(WiFi.localIP());
}
