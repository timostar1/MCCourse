#include "ArduinoJson.h"
#include "machine.h"
#include "Client.h"

// Левый передний
#define flfd  0
#define flpwm 0
#define flbd  4

// Левый задний
#define blfd  16
#define blpwm 1
#define blbd  17

// Правый задний
#define frfd  13
#define frpwm 2
#define frbd  12

// Правый передний
#define brfd  14
#define brpwm 3
#define brbd  27


#define pwmFreq 1000

char* ssid = "MGTS_GPON_FE9E";
char* pwd = "xCRQTeuF";

void setup() {
    Serial.begin(9600);
    
    ledcSetup(flpwm, pwmFreq, 8);
    ledcSetup(blpwm, pwmFreq, 8);
    ledcSetup(frpwm, pwmFreq, 8);
    ledcSetup(brpwm, pwmFreq, 8);

    ledcAttachPin(flfd, flpwm);
    pinMode(flbd, OUTPUT);

    ledcAttachPin(blfd, blpwm);
    pinMode(blbd, OUTPUT);

    ledcAttachPin(frfd, frpwm);
    pinMode(frbd, OUTPUT);

    ledcAttachPin(brfd, brpwm);
    pinMode(brbd, OUTPUT);
    
    setupWifi(ssid, pwd);
}

ctrl::Motor fl(flpwm, flbd);
ctrl::Motor bl(blpwm, blbd);
ctrl::Motor fr(frpwm, frbd);
ctrl::Motor br(brpwm, brbd);

ctrl::Machine m(&fl, &fr, &bl, &br);

int a[] = {150, 0, -150, 0};
int c = 0;

String json;

void loop() {
  Serial.print(json = get());
  if (json) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    Serial.print(" ");
    Serial.println(c++);
    int rx = doc["rightX"];
    int ry = doc["rightY"];
    int lx = doc["leftX"];
    m.y(ry);
    m.x(rx);
    m.rotate(lx);
    m.update();
  }
}
