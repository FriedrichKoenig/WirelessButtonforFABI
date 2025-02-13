//Examples can be adjusted if needed
#define KEY_ENTER 0x28 // Keyboard Return (ENTER)
//#define KEY_ENTER 0x1c // Keyboard Return (ENTER)
#define KEY_SPACE 0x2c // Keyboard Spacebar
#define KEY_RIGHT 0x4f // Keyboard Right Arrow
#define KEY_LEFT 0x50 // Keyboard Left Arrow
#define KEY_DOWN 0x51 // Keyboard Down Arrow
#define KEY_UP 0x52 // Keyboard Up Arrow

void buttonMapping(String txt){
  if(txt == "KEY_ENTER"){
      Keyboard.key_code_raw(KEY_ENTER);
      Serial.println("HENLOO");
  } else if (txt == "KEY_SPACE"){
      Keyboard.key_code_raw(KEY_SPACE);
  } else if (txt == "KEY_RIGHT"){
      Keyboard.key_code_raw(KEY_RIGHT);
  } else if (txt == "KEY_LEFT"){
      Keyboard.key_code_raw(KEY_LEFT);
  } else if (txt == "KEY_DOWN"){
      Keyboard.key_code_raw(KEY_DOWN);
  } else if (txt == "KEY_UP"){
      Keyboard.key_code_raw(KEY_UP);
  }
}





    
