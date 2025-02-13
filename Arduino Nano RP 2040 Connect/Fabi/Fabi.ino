#include "PluggableUSBHID.h"
#include "USBKeyboard.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoBLE.h>

//Keyboard
USBKeyboard Keyboard;

//Wifi
bool wifiActive = true;      // To track if WiFi is active

char ssid[] = "FABI";        //network SSID name
char pass[] = "asterics";    //network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

int maxButtons = 2;         // Default value for maximum number of buttons
String buttonActions[100];  // Array to store actions for each button

//BLE
// UUIDs for the service and characteristic
const char* PUCK_SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const char* PUCK_CHARACTERISTIC_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";

BLEDevice puckDevices[100]; //Store up to 100 Pucks
BLECharacteristic buttonCharacteristics[100];  
String puckNames[100];  // Store Puck.js names

int puckCount = 0; //How many Puck.js were already found

void setup() {
  Serial.begin(9600);
  while (!Serial);
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  
  digitalWrite(LEDR, HIGH);
  delay(200);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  delay(200);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);
  delay(200);
  digitalWrite(LEDB, LOW);
  setup_Wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(wifiActive){
    loop_Wifi();
  }
  while(!wifiActive){
    loop_BLE();
    Serial.println("!");
  }
}
