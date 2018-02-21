/*
  This sketch is made to be used with any ISO14443-A
  Author:  Pierre CHARLIER

  Compatible with KeyDuino 5.1 / KeyDuino NANO

  Join http://keyduino.forumsactifs.com/ to ask your questions, suggest your ideas, and show your projects!
*/

#include "KeyDuino.h"

KeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

void setup(void) {
  Serial.begin(115200);
  keyDuino.begin();
  Serial.println("KeyDuino is ready, please scan a tag.");
}

void loop(void) {
  uint8_t success_ISO14443A = keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success_ISO14443A) {
    if (uidLength == 4) {
      Serial.print("Mifare Classic identified: UID: ");
      keyDuino.PrintHex(uid, uidLength);
    }
    else if (uidLength == 7) {
      Serial.print("Mifare Ultralight identified: UID: ");
      keyDuino.PrintHex(uid, uidLength);
    }
    keyDuino.buzz(20); //for NORMAL
    keyDuino.led(100); //for NANO
    delay(250);
  }
}
