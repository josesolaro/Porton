#include <Arduino.h>

#include "Wifi.h"
#include "WiFiServer.h"
#include "WiFiClient.h"

#include "Porton.hpp"

MServer server;
RFID rfid;
Porton porton;

void intCallback()
{
  porton.Stop();
}

void setup() {
  Serial.begin(9600);

  porton.Init(&rfid, &server);

  pinMode(CLOSE_OUTPUT, OUTPUT);
  pinMode(OPEN_OUTPUT, OUTPUT);
  pinMode(APERTURA, INPUT_PULLUP);
  pinMode(CIERRE, INPUT_PULLUP);
  pinMode(CURRENT, ANALOG);
  porton.Stop();

  pinMode(LED_BUILTIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(APERTURA), intCallback, FALLING);
  attachInterrupt(digitalPinToInterrupt(CIERRE), intCallback, FALLING);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  porton.ListenCmd();
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
