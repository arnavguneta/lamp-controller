#ifndef LAMP_CONTROLLER_H
#define LAMP_CONTROLLER_H

#include <Arduino.h>
#include "HomeSpan.h"

extern bool lampPowerState;

void handleToggleLampPower(const String& eventBody);
void handleLampPower(const String& eventBody);
void handleBrightness(const String& eventBody);

void setLampPower(bool state, String brightness="low");

struct Lamp : Service::LightBulb
{
    SpanCharacteristic *power;

    Lamp() : Service::LightBulb()
    {
        power = new Characteristic::On();
    }

    boolean update()
    {
        setLampPower(power->getNewVal<bool>());
        lampPowerState = power->getNewVal<bool>();
        return (true);
    }
};

#endif
