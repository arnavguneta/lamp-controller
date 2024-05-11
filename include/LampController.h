#ifndef LAMP_CONTROLLER_H
#define LAMP_CONTROLLER_H

#include <Arduino.h>

void handleToggleLampPower(const String& eventBody);
void handleLampPower(const String& eventBody);

#endif
