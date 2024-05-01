// #include <Arduino.h>
// #include <WiFi.h>
// #include "Utils.h"
// #include "HttpUtils.h"
// // #include <driver/dac.h>

// const int YELLOW_LED_ANALOG = 25;
// const int WHITE_LED_ANALOG = 26;
// #define DAC_CH1 25

// void setup() {
//   Serial.begin(115200);
//   pinMode(LED_BUILTIN, OUTPUT);
//   Serial.println("Setup");
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi ..");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print('.');
//     digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
//     delay(1000);
//   }
//   Serial.println(WiFi.localIP());
//   // dac_output_enable(DAC_CHANNEL_1);
// }

// void loop() {
//   // Step through voltages, delay between levels
//   dacWrite(DAC_CH1, 0);
//   Serial.println("DAC Value 0");
//   delay(6000);

//   dacWrite(DAC_CH1, 87);
//   Serial.println("DAC Value 1v");
//   delay(6000);

//   dacWrite(DAC_CH1, 175);
//   Serial.println("DAC Value 2.2v");
//   delay(6000);

//   dacWrite(DAC_CH1, 255);
//   Serial.println("DAC Value 3.3v");
//   delay(6000);

// }
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include "WifiCredentials.h"

WiFiMulti WiFiMulti;
SocketIOclient socketIO;

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char* websockets_server_host = "localhost"; 
const uint16_t websockets_server_port = 3001;

const int YELLOW_LED_ANALOG = 25;
const int WHITE_LED_ANALOG = 26;
#define DAC_CH1 25


void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
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
            char * sptr = NULL;
            int id = strtol((char *)payload, &sptr, 10);
            Serial.printf("[IOc] get event: %s id: %d\n", payload, id);
            if(id) {
                payload = (uint8_t *)sptr;
            }
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, payload, length);
            if(error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return;
            }

            String eventName = doc[0];
            Serial.printf("[IOc] event name: %s\n", eventName.c_str());

            // Message Includes a ID for a ACK (callback)
            if(id) {
                // creat JSON message for Socket.IO (ack)
                DynamicJsonDocument docOut(1024);
                JsonArray array = docOut.to<JsonArray>();

                // add payload (parameters) for the ack (callback function)
                JsonObject param1 = array.createNestedObject();
                param1["now"] = millis();

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
    Serial.setDebugOutput(true);
    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.println();
    Serial.println();
    Serial.println();

      for(uint8_t t = 4; t > 0; t--) {
          Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
          Serial.flush();
          delay(1000);
      }

    WiFiMulti.addAP(ssid, password);

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    Serial.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    // server address, port and URL
    socketIO.begin("10.0.0.115", 3001, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;
void loop()
{
    socketIO.loop();

    uint64_t now = millis();

    // if(now - messageTimestamp > 2000) {
    //     messageTimestamp = now;

    //     // creat JSON message for Socket.IO (event)
    //     DynamicJsonDocument doc(1024);
    //     JsonArray array = doc.to<JsonArray>();

    //     // add evnet name
    //     // Hint: socket.on('event_name', ....
    //     array.add("event_name");

    //     // add payload (parameters) for the event
    //     JsonObject param1 = array.createNestedObject();
    //     param1["now"] = (uint32_t) now;

    //     // JSON to String (serializion)
    //     String output;
    //     serializeJson(doc, output);

    //     // Send event
    //     socketIO.sendEVENT(output);

    //     // Print JSON for debugging
    //     Serial.println(output);
    // }
}
