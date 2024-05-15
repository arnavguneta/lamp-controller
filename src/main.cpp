#include "HomeSpan.h"
#include "Constants.h"
#include "LampController.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include "WifiCredentials.h"
#include <driver/dac.h>

WiFiMulti WiFiMulti;
SocketIOclient socketIO;

void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
        Serial.printf("[IOc] Disconnected!\n");
        break;
    case sIOtype_CONNECT:
        Serial.printf("[IOc] Connected to url: %s\n", payload);

        // join default namespace (no auto join in Socket.IO V3)
        socketIO.send(sIOtype_CONNECT, "/");
        break;
    case sIOtype_EVENT:
    {
        char *sptr = NULL;
        int id = strtol((char *)payload, &sptr, 10);
        Serial.printf("[IOc] get event: %s id: %d\n", payload, id);
        if (id)
        {
            payload = (uint8_t *)sptr;
        }
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }

        String eventName = doc[0];
        String eventBody = doc[1];
        Serial.printf("[IOc] event name: %s\n", eventName.c_str());
        Serial.printf("[IOc] event: %s\n", eventBody.c_str());

        if (eventName == "toggleLampPower")
        {
            handleToggleLampPower(doc[1]);
        }
        else if (eventName == "lampPower")
        {
            handleLampPower(doc[1]);
        }
        else if (eventName == "brightness")
        {
            handleBrightness(doc[1]);
        }

        // Message Includes a ID for a ACK (callback)
        if (id)
        {
            // creat JSON message for Socket.IO (ack)
            JsonDocument docOut;
            JsonArray array = docOut.to<JsonArray>();

            // add payload (parameters) for the ack (callback function)
            JsonObject param1;
            param1["now"] = millis();
            array[0] = param1;

            // JSON to String (serializion)
            String output;
            output += id;
            serializeJson(docOut, output);

            // Send event
            socketIO.send(sIOtype_ACK, output);
        }
    }
    break;
    case sIOtype_ACK:
        Serial.printf("[IOc] get ack: %u\n", length);
        break;
    case sIOtype_ERROR:
        Serial.printf("[IOc] get error: %u\n", length);
        break;
    case sIOtype_BINARY_EVENT:
        Serial.printf("[IOc] get binary: %u\n", length);
        break;
    case sIOtype_BINARY_ACK:
        Serial.printf("[IOc] get binary ack: %u\n", length);
        break;
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Wait for serial port to connect. Needed for native USB port only
    }
    // Serial.setDebugOutput(true);
    pinMode(LED_BUILTIN, OUTPUT);

    for (uint8_t t = 2; t > 0; t--)
    {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    while (WiFiMulti.run() != WL_CONNECTED)
    {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    Serial.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    // server address, port and URL
    socketIO.begin(websockets_server_host, websockets_server_port, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);

    homeSpan.begin(Category::Lighting, "Room Lamp 1");

    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();

    new Lamp();
}

void loop()
{
    socketIO.loop();
    homeSpan.poll();
}
