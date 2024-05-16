#ifndef LAMP_CONTROLLER_H
#define LAMP_CONTROLLER_H

#include <Arduino.h>
#include "HomeSpan.h"
#include <map>

extern std::map<String, int> brightnessMap;

struct Lamp : Service::LightBulb
{
    SpanCharacteristic *power;
    SpanCharacteristic *level;
    String brightnessCategory;

    Lamp() : Service::LightBulb()
    {
        brightnessCategory = "";
        power = new Characteristic::On();
        level = new Characteristic::Brightness(25);
        level->setRange(1, 100, 1);
    }

    void handleToggleLampPower(const String &eventBody);
    void handleLampPower(const String &eventBody);
    void handleBrightness(const String &eventBody);

    void setLampPower(bool state, String brightness = "low");
    String categorizeBrightness(int value);
    int categoryToBrightness(String category);

    boolean update()
    {
        String newBrightnessCategory = categorizeBrightness(level->getNewVal());
        if (newBrightnessCategory.compareTo(brightnessCategory) == 0 && power->getNewVal<bool>() == power->getVal<bool>())
            return (true);
        brightnessCategory = newBrightnessCategory;
        setLampPower(power->getNewVal<bool>(), newBrightnessCategory);
        return (true);
    }

    SpanCharacteristic *getPowerCharacteristic()
    {
        return power;
    }

    SpanCharacteristic *getBrightnessCharacteristic()
    {
        return level;
    }

    void setPowerCharacteristic(bool powerVal)
    {
        power->setVal(powerVal);
    }

    void setBrightnessCharacteristic(String brightnessCategory)
    {
        int brightnessVal = categoryToBrightness(brightnessCategory);
        level->setVal(brightnessVal);
        this->brightnessCategory = brightnessCategory;
    }
};

#endif
