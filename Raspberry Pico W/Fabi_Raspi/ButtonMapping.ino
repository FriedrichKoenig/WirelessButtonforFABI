// -------------------- Key Kodes --------------------
#define KEY_ENTER 0x28 // Keyboard Return (ENTER)
#define KEY_SPACE 0x2C // Keyboard Spacebar
#define KEY_RIGHT 0x4F // Keyboard Right Arrow
#define KEY_LEFT  0x50 // Keyboard Left Arrow
#define KEY_DOWN  0x51 // Keyboard Down Arrow
#define KEY_UP    0x52 // Keyboard Up Arrow

// -------------------- Funcion to map the button actions --------------------
void buttonMapping(String txt) {
  if(txt == "KEY_ENTER"){
      Keyboard.consumerPress(KEY_ENTER);
  } else if (txt == "KEY_SPACE"){
      Keyboard.consumerPress(KEY_SPACE);
  } else if (txt == "KEY_RIGHT"){
      Keyboard.consumerPress(KEY_RIGHT);
  } else if (txt == "KEY_LEFT"){
      Keyboard.consumerPress(KEY_LEFT);
  } else if (txt == "KEY_DOWN"){
      Keyboard.consumerPress(KEY_DOWN);
  } else if (txt == "KEY_UP"){
      Keyboard.consumerPress(KEY_UP);
  }
}
