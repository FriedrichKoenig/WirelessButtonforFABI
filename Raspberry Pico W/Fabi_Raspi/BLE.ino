// -------------------- BLE SETUP/START --------------------
void setup_BLE() {
  // Initialize all BTstack callbacks we need
  BTstack.setBLEAdvertisementCallback(advertisementCallback);
  BTstack.setBLEDeviceConnectedCallback(deviceConnectedCallback);
  BTstack.setBLEDeviceDisconnectedCallback(deviceDisconnectedCallback);
  BTstack.setGATTServiceDiscoveredCallback(gattServiceDiscovered);
  BTstack.setGATTCharacteristicDiscoveredCallback(gattCharacteristicDiscovered);
  BTstack.setGATTCharacteristicNotificationCallback(gattCharacteristicNotification);
  //BTstack.setGATTCharacteristicReadCallback(gattReadCallback);
  //BTstack.setGATTCharacteristicWrittenCallback(gattWrittenCallback);
  BTstack.setGATTCharacteristicSubscribedCallback(gattSubscribedCallback);
  //BTstack.setGATTCharacteristicIndicationCallback(gattCharacteristicIndication);

  // Start BTstack
  BTstack.setup();
 
  // Begin scanning for BLE advertisements
  puckCount = 0;
  BTstack.bleStartScanning();
 
  Serial.println("BTstack BLE scanning started...");
}
 
// -------------------- BLE CALLBACKS --------------------
 
// Called whenever an advertisement is received
void advertisementCallback(BLEAdvertisement *advertisement) {
  // If we already have enough connected Puck.js, ignore
  if (puckCount >= maxButtons) return;
 
  // Check if this advertisement has our Puck.js service
  if (advertisement->containsService(&puckServiceUUID)) {
    // Attempt to connect
    Serial.println("Found a Puck.js device advertising our service!");
 
    // Store the device’s name
    currentConnectIndex = puckCount;
    puckNames[currentConnectIndex] = advertisement->getBdAddr()->getAddressString();
    Serial.print("Assigning device index ");
    Serial.print(currentConnectIndex);
    Serial.print(" name: ");
    Serial.println(puckNames[currentConnectIndex]);
    // Stop scanning so we can connect
    BTstack.bleStopScanning();
    // Connect with a 10s timeout
    BTstack.bleConnect(advertisement, 1000);
  }
}
 
// Called when the connection attempt completes
void deviceConnectedCallback(BLEStatus status, BLEDevice *device) {
  if (status == BLE_STATUS_OK) {
    // Connection success
    Serial.println("Device connected!");
    puckDevices[currentConnectIndex] = *device;
    puckConnected[currentConnectIndex] = true;
 
    // Start service discovery
    puckServiceFound[currentConnectIndex] = false;
    puckCharFound[currentConnectIndex] = false;
    //BuhBuhBuhdevice->discoverGATTServices();
    //Serial.println(currentConnectIndex);
    Serial.print("Services: ");
    Serial.println(puckDevices[currentConnectIndex].discoverGATTServices());
    //Serial.println(device->discoverGATTServices());
    puckDevices[currentConnectIndex].discoverGATTServices();
    
  } else if (status == BLE_STATUS_CONNECTION_TIMEOUT) {
    Serial.println("Connection timeout or error while connecting!");
    // Resume scanning for more Puck.js
    BTstack.bleStartScanning();
  }
  else {
    Serial.print("Unknown connect error: ");
    Serial.println(status);
    // Resume scanning
    BTstack.bleStartScanning();
  }
}
 
// Called if device gets disconnected
void deviceDisconnectedCallback(BLEDevice *device) {
  // Find which device index this is
  int disconnectedIndex = -1;
  for (int i = 0; i <= puckCount; i++) {
    if (puckDevices[i].getHandle() == device->getHandle()){
      disconnectedIndex = i;
      break;
    }
  }
 
  Serial.println("Device disconnected, scanning again...");
  if (disconnectedIndex != -1) {
    puckConnected[disconnectedIndex] = false;
  }
  // Keep scanning for more devices
  BTstack.bleStartScanning();
}
 
// Service discovery results
void gattServiceDiscovered(BLEStatus status, BLEDevice *device, BLEService *service) {
  Serial.print("Status: ");
  Serial.println(status);
  if (status == BLE_STATUS_OK) {
    Serial.print("Service discovered: ");
    Serial.println(service->getUUID()->getUuidString());
 
    // Check if it’s the Puck.js service
    if (service->matches(&puckServiceUUID)) {
      Serial.println("Puck.js service found!");
      // Store service
      for (int i = 0; i < puckCount + 1; i++) {
        if (puckDevices[i].getHandle() == device->getHandle()){
          puckServices[i] = *service;
          puckServiceFound[i] = true;
          break;
        }
      }
    }
  } 
  else if (status == BLE_STATUS_DONE) {
    Serial.println("Service discovery finished.");
 
    // If we found the service, discover its characteristic(s).
    for (int i = 0; i < puckCount + 1; i++) {
      //if (puckDevices[i].isSameDevice(device) && puckServiceFound[i]) {
      if (puckDevices[i].getHandle() == device->getHandle()){
        device->discoverCharacteristicsForService(&puckServices[i]);
      }
    }
  }
  else {
    Serial.println("Service discovery error.");
  }
}
 
// Characteristic discovery results
void gattCharacteristicDiscovered(BLEStatus status, BLEDevice *device, BLECharacteristic *characteristic) {
  if (status == BLE_STATUS_OK) {
    Serial.print("Characteristic discovered: ");
    Serial.print(characteristic->getUUID()->getUuidString());
    Serial.print(", handle 0x");
    Serial.println(characteristic->getCharacteristic()->value_handle, HEX);

    // Check if it's the Puck.js button characteristic
    if (characteristic->matches(&puckCharacteristicUUID)) {
      Serial.println("Puck.js characteristic found!");
      for (int i = 0; i < puckCount + 1; i++) {
        if (puckDevices[i].getHandle() == device->getHandle()){
          puckChars[i] = *characteristic;
          puckCharFound[i] = true;
          Serial.println(puckDevices[i].getHandle());
        }
      }
    }
  }
  else if (status == BLE_STATUS_DONE) {
    Serial.println("Characteristic discovery finished.");
 
    // Subscribe for notifications if we found the characteristic
    for (int i = 0; i < puckCount + 1; i++) {
      //if (puckDevices[i].isSameDevice(device) && puckCharFound[i]) {
      if (puckDevices[i].getHandle() == device->getHandle()) {
        // Subscribe
        //BLEStatus subscribeStatus = 
        device->subscribeForNotifications(&puckChars[i]);
        if (status == BLE_STATUS_OK) {
          Serial.println("Subscription to notifications requested.");
        }
      }
    }
 
    // Increase the puckCount since we have successfully connected & discovered
    // for the current device index
    puckCount++;
    Serial.print("Total devices connected: ");
    Serial.println(puckCount);
    // If we can still connect more devices, resume scanning
    if (puckCount < maxButtons) {
      BTstack.bleStartScanning();
    }
  }
  else {
    Serial.println("Characteristic discovery error.");
  }
}
 
// Called after we attempt to subscribe
void gattSubscribedCallback(BLEStatus status, BLEDevice *device) {
  if (status == BLE_STATUS_OK) {
    Serial.println("Successfully subscribed to Puck.js notifications!");
  } else {
    Serial.print("Subscription failed, status = ");
    Serial.println(status);
  }
}
 
// Called when we receive a notification from a characteristic
void gattCharacteristicNotification(BLEDevice *device, uint16_t value_handle, uint8_t *value, uint16_t length) {
  //'value' is the data from the remote device
  // We want the second bit of the first byte (0x02):
  (void) device;
  (void) value_handle;
  (void) length;
  Serial.print("Notification: ");
  Serial.println((const char *)value);
  if (length > 0) {
    char* v = (char *)value;
    Serial.print("Value: ");
    Serial.println(v[4]);
    if (v[4] == '1' and currently_pressed == false) {
      currently_pressed = true;
      // Find which device index this is
      int devIndex = -1;
      for (int i = 0; i < puckCount; i++) {
        if (puckDevices[i].getHandle() == device->getHandle()) {
          devIndex = i;
          break;
        }
      }
      if (devIndex != -1) {
        Serial.print(puckNames[devIndex]);
        Serial.println(" button pressed");
 
        // Retrieve the action string
        String buttonText = buttonActions[devIndex];
        // If it starts with KEY_, treat as special key
        if (buttonText.startsWith("KEY_")) {
          buttonMapping(buttonText);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
        } else {
          // Otherwise, type the text
          Keyboard.print(buttonText.c_str());
          Serial.println(buttonText);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    } else if (v[4] == '0' and currently_pressed == true){
      currently_pressed = false;
      Serial.println("Not Pressed");
    } else {
      Serial.println("Something Strange has happened");
    }
  }
}
