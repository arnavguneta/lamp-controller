#ifndef LAMP_CONTROLLER_H
#define LAMP_CONTROLLER_H

#include <Arduino.h>
#include "HomeSpan.h"
#include <map>

extern bool lampPowerState;
extern std::map<String, int> brightnessMap;

void handleToggleLampPower(const String& eventBody);
void handleLampPower(const String& eventBody);
void handleBrightness(const String& eventBody);

void setLampPower(bool state, String brightness="low");

String categorize_brightness(int value);

struct Lamp : Service::LightBulb
{
    SpanCharacteristic *power;
    SpanCharacteristic *level;

    Lamp() : Service::LightBulb()
    {
        power = new Characteristic::On();
        level = new Characteristic::Brightness(33);
        level->setRange(33, 100, 33);
    }

    boolean update()
    {
        setLampPower(power->getNewVal<bool>(), categorize_brightness(level->getNewVal()));
        lampPowerState = power->getNewVal<bool>();
        return (true);
    }
};

#endif
