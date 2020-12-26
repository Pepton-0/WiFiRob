// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Const.h>

bool ledStateFR = 0;
bool ledStateFL = 0;
bool ledStateBR = 0;
bool ledStateBL = 0;
const int ledPinFR = 12;
const int ledPinFL = 4;
const int ledPinBR = 14;
const int ledPinBL = 5;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const Const cValues;

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        if (strcmp((char*)data, "FORWARD") == 0) {
            ledStateFR = HIGH;
            ledStateFL = HIGH;
            ledStateBR = LOW;
            ledStateBL = LOW;
        }
        if (strcmp((char*)data, "BACKWARD") == 0) {
            ledStateFR = LOW;
            ledStateFL = LOW;
            ledStateBR = HIGH;
            ledStateBL = HIGH;
        }
        if (strcmp((char*)data, "RIGHT") == 0) {
            ledStateFR = HIGH;
            ledStateFL = LOW;
            ledStateBR = LOW;
            ledStateBL = HIGH;
        }
        if (strcmp((char*)data, "TL") == 0) {
            ledStateFR = LOW;
            ledStateFL = HIGH;
            ledStateBR = HIGH;
            ledStateBL = LOW;
        }
        if (strcmp((char*)data, "STOP") == 0) {
            ledStateFR = LOW;
            ledStateFL = LOW;
            ledStateBR = LOW;
            ledStateBL = LOW;
        }
    }
}
void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type,
    void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_DATA) {
        handleWebSocketMessage(arg, data, len);
    }
}
void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}
void setup() {
    Serial.begin(115200);
    pinMode(ledPinFR, OUTPUT);
    pinMode(ledPinFL, OUTPUT);
    pinMode(ledPinBR, OUTPUT);
    pinMode(ledPinBL, OUTPUT);
    digitalWrite(ledPinFR, LOW);
    digitalWrite(ledPinFL, LOW);
    digitalWrite(ledPinBR, LOW);
    digitalWrite(ledPinBL, LOW);
    WiFi.begin(cValues.ssid, cValues.password);
    Serial.println(WiFi.localIP());
    initWebSocket();
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        // request->send_P(200, "text/html", index_html);
        request->send(200, "text/html", cValues.index.c_str());
        });
    server.begin();
}
void loop() {
    ws.cleanupClients();
    digitalWrite(ledPinFL, ledStateFL);
    digitalWrite(ledPinBL, ledStateBL);
    digitalWrite(ledPinFR, ledStateFR);
    digitalWrite(ledPinBR, ledStateBR);
}