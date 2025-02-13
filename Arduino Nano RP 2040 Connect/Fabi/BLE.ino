void setup_BLE(){
  digitalWrite(LED_BUILTIN, LOW);
  // Start BLE central mode
  if (!BLE.begin()) {
    Serial.println("Failed to start BLE!");
    while (1);
  }

  Serial.println("BLE Central - Arduino RP2040 Connect");
  BLE.scan();  // Start scanning for peripherals
}

void loop_BLE(){
  // If less than maxButtons connected, keep scanning
  if (puckCount < maxButtons) {
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
      String deviceName = peripheral.localName();

      Serial.print("Found device: ");
      Serial.println(deviceName);

      // Check if it's advertising the correct service UUID
      if (peripheral.advertisedServiceUuid() == PUCK_SERVICE_UUID) {
        Serial.println("Found a Puck.js device!");
        BLE.stopScan();  // Stop scanning to connect

        if (peripheral.connect()) {
          Serial.println("Connected to Puck.js");

          // Discover the service and characteristic
          BLEService puckService = peripheral.service(PUCK_SERVICE_UUID);
          if (peripheral.discoverService(PUCK_SERVICE_UUID)) {
            BLECharacteristic buttonCharacteristic = peripheral.characteristic(PUCK_CHARACTERISTIC_UUID);
            
            if (!buttonCharacteristic || !buttonCharacteristic.canSubscribe() || !buttonCharacteristic.subscribe()) {
              Serial.println("Failed to subscribe!");
              peripheral.disconnect();
              return;
            } else {
              Serial.println("Subscribed to button notifications");

              // Store device and characteristic
              puckDevices[puckCount] = peripheral;
              buttonCharacteristics[puckCount] = buttonCharacteristic;
              puckNames[puckCount] = deviceName;

              puckCount++;
              Serial.print("Connected to ");
              Serial.println(deviceName);
            }
          }
        } else {
          Serial.println("Failed to connect!");
        }

        BLE.scan();  // Continue scanning for more devices
      }
    }
  }
  checkForButtonPresses(); //The Keyboard Action
}
