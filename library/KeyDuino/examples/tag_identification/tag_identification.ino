/*
This sketch is made to be used with any ISO14443-A tag (like Mifare), to read its ID.

Author: Raymond Borenstein - CITC-EuraRFID

Compatible with KeyDuino NORMAL / KeyDuino NANO

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
  uint8_t success;
  String readID;
  success = keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success){
    readID = keyDuino.convertUintToString(uid, uidLength);
    Serial.print("Tag found: ");
    Serial.println(readID);
    keyDuino.buzz(20); //for NORMAL
    keyDuino.led(100); //for NANO
    delay(250);
  }
  delay(1);
}
