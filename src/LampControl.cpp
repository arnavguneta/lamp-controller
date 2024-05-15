#include "LampController.h"
#include "Constants.h"

#include <ArduinoJson.h>
#include <Arduino.h>

std::map<String, int> brightnessMap = {{"low", 87}, {"medium", 150}, {"high", 255}};

String Lamp::categorizeBrightness(int value) 
{
    if (value <= 33)
        return "low";
    else if (value > 33 && value <= 66)
        return "medium";
    else
        return "high";
}

int Lamp::categoryToBrightness(String category) {
    if (category.compareTo("low") == 0)
        return 33;
    else if (category.compareTo("medium") == 0)
        return 66;
    else
        return 100;
} 

void Lamp::setLampPower(bool state, String brightness)
{
    Lamp::setBrightnessCharacteristic(brightness);
    Lamp::setPowerCharacteristic(state);
    bool lampPowerState = Lamp::getPowerCharacteristic()->getVal<bool>();
    Serial.printf("[LampController] Power state: %d -> %d, brightness %s\n", lampPowerState, state, brightness.c_str());

    if (lampPowerState)
    {
        dacWrite(YELLOW_LED_ANALOG, brightnessMap[brightness]);
        dacWrite(WHITE_LED_ANALOG, brightnessMap[brightness]);
    }
    else
    {
        dacWrite(YELLOW_LED_ANALOG, 0);
        dacWrite(WHITE_LED_ANALOG, 0);
    }
}

void Lamp::handleToggleLampPower(const String &eventBody)
{
    bool lampPowerState = Lamp::getPowerCharacteristic()->getVal<bool>();    
    setLampPower(!lampPowerState);
}

void Lamp::handleLampPower(const String &eventBody)
{
    JsonDocument doc;
    deserializeJson(doc, eventBody);
    setLampPower(doc["state"]);
}

void Lamp::handleBrightness(const String &eventBody)
{
    JsonDocument doc;
    deserializeJson(doc, eventBody);
    setLampPower(true, doc["level"]);
}
