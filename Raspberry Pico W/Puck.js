const SERVICE_UUID = '6e400001-b5a3-f393-e0a9-e50e24dcca9e'; // Service UUID
const CHARACTERISTIC_UUID = '6e400003-b5a3-f393-e0a9-e50e24dcca9e'; // Characteristic UUID

let buttonState = 0; // 0 = Released, 1 = Pressed

// Function to set up services and start advertising
function setupBLE() {
  // Set up GATT services and characteristics
  NRF.setServices({
    '6e400001-b5a3-f393-e0a9-e50e24dcca9e': {
      '6e400031-b5a3-f393-e0a9-e50e24dcca9e': {
        value: [buttonState],        // Initial value
        readable: true,    // Allow reading the value
        writable: false,   // Disable writing to the characteristic
        notify: true       // Enable notifications
      }
    }
  }, { advertise: [SERVICE_UUID], uart: false });

  console.log("BLE services set up and advertising started.");
}

// Function to notify the connected device of button state changes
function notifyButtonState() {
  NRF.updateServices({
    '6e400001-b5a3-f393-e0a9-e50e24dcca9e': {
      '6e400003-b5a3-f393-e0a9-e50e24dcca9e': {
        value: [buttonState],        // Initial value
        readable: true,    // Allow reading the value
        writable: false,   // Disable writing to the characteristic
        notify: true       // Enable notifications
      }
    }
  });
  //console.log("Notification sent: " + buttonState);
}

// Restart BLE and set up services
function restartBLE() {
  console.log("Restarting BLE...");
  //NRF.disconnect(); // Disconnect any connected devices
  NRF.restart(() => {
    setupBLE(); // Set up BLE after restart
    console.log("BLE restarted.");
  });
}

// Watch for button press
setWatch(() => {
  buttonState = 1;
  console.log(buttonState); //Raspi recognizes only this
  notifyButtonState();
  LED1.write(buttonState);
}, BTN, { edge: "rising", debounce: 50, repeat: true });

// Watch for button release
setWatch(() => {
  buttonState = 0;
  console.log(buttonState); //Raspi recognizes only this
  notifyButtonState();
  LED1.write(buttonState);
}, BTN, { edge: "falling", debounce: 50, repeat: true });

// Main execution
//restartBLE();
