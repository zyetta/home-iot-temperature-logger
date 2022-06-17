#ifndef _MQTTCONTROLLER_H
#define _MQTTCONTROLLER_H

#include <PubSubClient.h>

void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif
