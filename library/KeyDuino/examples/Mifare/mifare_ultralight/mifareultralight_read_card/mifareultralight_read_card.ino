/*
  This sketch is made to be used with a Mifare Ultralight card, and serves as an example to read page 4.

  Author: Pierre CHARLIER

  Compatible with KeyDuino 5.1

  Join http://keyduino.forumsactifs.com/ to ask your questions, suggest your ideas, and show your projects!
*/

#include <KeyDuino.h>

KeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

void setup(void) {
  Serial.begin(115200);

  keyDuino.begin();

  keyDuino.SAMConfig();

  Serial.println("Waiting for a Mifare Ultralight tag to read.");
}

void loop(void) {
  uint8_t success;
  uint8_t data[32];
  //Try to read card UID
  success = keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    if (uidLength == 7) {
      Serial.print("Mifare Ultralight identified: UID: ");
      keyDuino.PrintHex(uid, uidLength);

      success = keyDuino.mifareultralight_ReadPage (4, data);  // first general-purpose user page (#4)
      if (success)
      {
        Serial.print("Page 4 : ");
        keyDuino.PrintHexChar(data, 4);
        Serial.println("");
      }

      delay(500);
      Serial.println("Operation complete.");
      Serial.println("Press a key for next operation.");
      while (!Serial.available());
      while (Serial.available()) Serial.read();
      Serial.println("Waiting for a Mifare Ultralight tag to read.");
    } else {
      Serial.println("Detected tag is not Mifare Ultralight.");
      delay(500);
    }
  }
}



