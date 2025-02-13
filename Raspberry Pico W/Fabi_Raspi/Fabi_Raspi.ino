/*
 * Raspberry Pi Pico W code for Wi-Fi AP + BLE (BTstack).
 * 
 * Functionality:
 *  - Runs a Wi-Fi AP on startup.
 *  - Hosts a web page to set "maxButtons" and each button's key action.
 *  - After clicking "Start FABI", disables Wi-Fi and starts BLE scanning.
 *  - Connects to Puck.js devices advertising the given service/characteristic.
 *  - On notifications (button press), sends the configured text or keystroke over USB.
 */
 
// -------------------- INCLUDES --------------------
#include <BTstackLib.h>           // BTstack for BLE
#include <SPI.h>
//#include "PluggableUSBHID.h"
//#include "USBKeyboard.h"
#include <Keyboard.h> 
// For Wi-Fi on Pico W (Earle Philhower core):
#include <WiFi.h>                 // Instead of WiFiNINA.h
//#include <WiFiClient.h>
// -------------------- KEYBOARD SETUP --------------------
//USBKeyboard Keyboard;
 
// -------------------- USER-CONFIGURABLE WIFI CREDENTIALS --------------------
char ssid[] = "FABI";        // Access Point SSID
char pass[] = "asterics";    // Access Point password
 
// -------------------- GLOBALS --------------------
 
// Wi-Fi
bool wifiActive = true;      // Tracks if WiFi is active (AP mode)
int status; // = WL_IDLE_STATUS;
WiFiServer server(80);
 
// Number of buttons to connect to
int maxButtons = 3;      
String buttonActions[100];  // Action strings for each “button”
 
// BLE (BTstack) Definitions
// The Puck.js UUIDs as 128-bit. BTstack uses a UUID class:
UUID puckServiceUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
UUID puckCharacteristicUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e");
 
// We'll allow up to 100 discovered Puck.js devices
BLEDevice  puckDevices[100];
bool       puckConnected[100];          // Keep track of whether device is connected
BLEService puckServices[100];           // The service object for each device
BLECharacteristic puckChars[100];       // The characteristic object for each device
bool puckServiceFound[100];            
bool puckCharFound[100];
String puckNames[100];                  // Puck.js device name from advertisement
 
int puckCount = 0;                      // How many Puck.js have been connected so far
int currentConnectIndex = -1;           // Which array index is currently connecting
 
// Forward declarations of our BLE callbacks:
void advertisementCallback(BLEAdvertisement *advertisement);
void deviceConnectedCallback(BLEStatus status, BLEDevice *device);
void deviceDisconnectedCallback(BLEDevice *device);
void gattServiceDiscovered(BLEStatus status, BLEDevice *device, BLEService *service);
void gattCharacteristicDiscovered(BLEStatus status, BLEDevice *device, BLECharacteristic *characteristic);
void gattSubscribedCallback(BLEStatus status, BLEDevice *device);
void gattCharacteristicNotification(BLEDevice *device, uint16_t value_handle, uint8_t *value, uint16_t length);
void gattReadCallback(BLEStatus status, BLEDevice *device, uint8_t *value, uint16_t length);
void gattWrittenCallback(BLEStatus status, BLEDevice *device);


const char *button_pressed = "[J1  \n"; 
bool currently_pressed = false;
 
// -------------------- SETUP --------------------
void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  while (!Serial);
 
  // Initialize built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  
 
  // Start Wi-Fi as Access Point
  setup_Wifi(); 
  //setup_BLE();
}
 
// -------------------- MAIN LOOP --------------------
void loop() {
  // As long as Wi-Fi (AP mode) is active, serve the config page
  while (wifiActive) {
    loop_Wifi();
  }
 
  // Once Wi-Fi is disabled and BLE is started, let BTstack handle BLE
  while (!wifiActive) {
    // The BTstack library is event/callback-driven, but you
    // must regularly call BTstack.loop() in your main loop.
    BTstack.loop();
  }
}
 

 

 
