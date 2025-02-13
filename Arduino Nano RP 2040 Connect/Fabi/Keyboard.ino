void checkForButtonPresses() {
  // Check for updates from each connected Puck.js device
  for (int i = 0; i < puckCount; i++) {
    if (puckDevices[i].connected() && buttonCharacteristics[i].valueUpdated()) {
      byte value = 0;
      buttonCharacteristics[i].readValue(value);

      // Check if the left button (second bit) was pressed
      if (value & 0x02) {
        Serial.print(puckNames[i]);
        Serial.println(" button pressed");

        // Print the corresponding button action to USB using c_str()
        String buttonText = buttonActions[i].c_str();
        if (buttonText.startsWith("KEY_")){
          buttonMapping(buttonText);
          digitalWrite(LED_BUILTIN, HIGH); //RGB LED not compatibl with BLE
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
        } else {
          Keyboard.printf(buttonActions[i].c_str()); // Convert String to const char*
          Serial.println(buttonText);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }
  }
}
