#include "LampController.h"
#include "Constants.h"

#include <ArduinoJson.h>
#include <Arduino.h>

bool lampPowerState = false;

std::map<String, int> brightnessMap = {{"low", 87}, {"medium", 150}, {"high", 255}};

String categorize_brightness(int value) 
{
    if (value < 33)
        return "low";
    else if (value >= 33 && value <= 66)
        return "medium";
    else
        return "high";
}

void setLampPower(bool state, String brightness)
{
    Serial.printf("[LampController] Power state: %d -> %d\n", lampPowerState, state);
    lampPowerState = state;
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

void handleToggleLampPower(const String &eventBody)
{
    setLampPower(!lampPowerState);
}

void handleLampPower(const String &eventBody)
{
    JsonDocument doc;
    deserializeJson(doc, eventBody);
    setLampPower(doc["state"]);
}

void handleBrightness(const String &eventBody)
{
    JsonDocument doc;
    deserializeJson(doc, eventBody);
    setLampPower(true, doc["level"]);
}