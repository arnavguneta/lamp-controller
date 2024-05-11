#include "LampController.h"
#include "Constants.h"

#include <ArduinoJson.h>
#include <Arduino.h>
#include <map>

bool lampPowerState = false;

std::map<String, int> brightnessMap = {{"low", 87}, {"medium", 175}, {"high", 255}};

void setLampPower(bool state, String brightness = "high")
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
    setLampPower(true, doc["brightness"]);
}