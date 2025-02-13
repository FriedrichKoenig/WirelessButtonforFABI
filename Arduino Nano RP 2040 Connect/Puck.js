// Service UUID and Characteristic UUID (using a valid 16-bit alias)
const SERVICE_UUID = '6e400001-b5a3-f393-e0a9-e50e24dcca9e';
const CHARACTERISTIC_UUID = '6e400003-b5a3-f393-e0a9-e50e24dcca9e';

var state = 1;
function press() {
  /* switch(state) {
    case 0:
      state = 1;
      LED1.write(1);
      break;
    case 1:
      state = 0;
      LED1.write(0);
      break;
  } */
  
  LED1.write(1);
  setTimeout(function(){
    LED1.write(0);
  }, 100);
  sendButtonState();
}

// Detect button press and release
setWatch(press, BTN1, { edge: 'rising', debounce: 50, repeat: true });

// Start advertising with specified service
NRF.setAdvertising({}, {
  name: "Puck.js Button 1",
  connectable: true,
  services: [SERVICE_UUID] // Ensure the service UUID is included
});

// Function to send button state over BLE
function sendButtonState(state) {
  // Advertise with services that notify button press state
  NRF.setServices({
    '6e400001-b5a3-f393-e0a9-e50e24dcca9e': {
      '6e400003-b5a3-f393-e0a9-e50e24dcca9e': {
        value: [state],
        readable: true,
        notify: true
      }
    }
  });
  NRF.setAdvertising({}, {
  name: "Puck.js Button 2",
  connectable: true,
  services: [SERVICE_UUID], // Ensure the service UUID is included
  manufacturer: 0x0590,
  manufacturerData: JSON.stringify({state}),
});
}
