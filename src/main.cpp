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
#include <ArduinoWebsockets.h>
#include "WifiCredentials.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char* websockets_server_host = "localhost"; 
const uint16_t websockets_server_port = 3001;

const int YELLOW_LED_ANALOG = 25;
const int WHITE_LED_ANALOG = 26;
#define DAC_CH1 25

using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

WebsocketsClient client;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Wait for serial port to connect. Needed for native USB port only
    }

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Setup");

    // Connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(1000);
    }
    Serial.println("WiFi connected");

    // run callback when messages are received
    client.onMessage(onMessageCallback);
    
    // run callback when events are occuring
    client.onEvent(onEventsCallback);

    // Connect to server
    client.connect(websockets_server_host, websockets_server_port, "/");

    // Send a message
    client.send("Hello Server");

    // Send a ping
    client.ping();
}

void loop()
{
    client.poll();

    // Step through voltages, delay between levels
    // dacWrite(DAC_CH1, 0);
    // Serial.println("DAC Value 0");
    // delay(6000);

    // dacWrite(DAC_CH1, 87);
    // Serial.println("DAC Value 1v");
    // delay(6000);

    // dacWrite(DAC_CH1, 175);
    // Serial.println("DAC Value 2.2v");
    // delay(6000);

    // dacWrite(DAC_CH1, 255);
    // Serial.println("DAC Value 3.3v");
    // delay(6000);
}
