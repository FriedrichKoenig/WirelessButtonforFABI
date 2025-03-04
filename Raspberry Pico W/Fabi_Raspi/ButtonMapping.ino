// -------------------- Key Kodes --------------------
#define KEY_ENTER 176  // Keyboard Return (ENTER)
#define KEY_KP_DOT 235 // Keyboard Spacebar
#define KEY_RIGHT 215 // Keyboard Right Arrow
#define KEY_LEFT  216 // Keyboard Left Arrow
#define KEY_DOWN  217 // Keyboard Down Arrow
#define KEY_UP    218 // Keyboard Up Arrow
#define KEY_F5    198 // Keyboard F5

// -------------------- Funcion to map the button actions --------------------
void buttonMapping(String txt) {
  if(txt == "KEY_ENTER"){
      Keyboard.press(KEY_ENTER);
      delay(10);
      Keyboard.release(KEY_ENTER);
  } else if (txt == "KEY_KP_DOT"){
      Keyboard.press(KEY_KP_DOT);
      delay(10);
      Keyboard.release(KEY_KP_DOT);
  } else if (txt == "KEY_RIGHT"){
      Keyboard.press(KEY_RIGHT);
      delay(10);
      Keyboard.release(KEY_RIGHT);
  } else if (txt == "KEY_LEFT"){
      Keyboard.press(KEY_LEFT);
      delay(10);
      Keyboard.release(KEY_LEFT);
  } else if (txt == "KEY_DOWN"){
      Keyboard.press(KEY_DOWN);
      delay(10);
      Keyboard.release(KEY_DOWN);
  } else if (txt == "KEY_UP"){
      Keyboard.press(KEY_UP);
      delay(10);
      Keyboard.release(KEY_UP);
  } else if (txt == "KEY_F5"){
      Keyboard.press(KEY_F5);
      delay(10);
      Keyboard.release(KEY_F5);
  }
}
