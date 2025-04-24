#ifndef GLOBALS_H
#define GLOBALS_H

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Ticker.h>
#include <Arduino.h>

// WiFi credentials
extern const char* ssid;
extern const char* password;

// Status buffer
extern char Status[200];

// Server & clients
#define MAX_CLIENTS 5
extern WiFiServer server;
extern WiFiClient clients[MAX_CLIENTS];
extern unsigned long lastActiveTime[MAX_CLIENTS];

// Timing
#define Time_Delay 10000  // 10 seconden timeout

// I²C for SHT31
#define SDA_PIN D2
#define SCL_PIN D1

// LED & RGB pins
#define LED_PIN   D5
#define RED_PIN   D6
#define GREEN_PIN D7
#define BLUE_PIN  D8

// Knop-flags
extern int Print_Knop;
extern int PiPrint_Knop;

// SHT31-object
extern Adafruit_SHT31 sht31;

// Timer voor LED uitschakeling
extern Ticker ledTimer;

#endif // GLOBALS_H
