#include "KeyDuino.h"

KeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;
uint8_t buf[4];
uint8_t password[4] = {0x12, 0x34, 0x56, 0x78};

void setup(void) {
  Serial.begin(115200);
  keyDuino.begin();
  Serial.println("NTAG21x password writer");
}


void loop(void) {
  if (keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    String readID = keyDuino.convertUintToString(uid, uidLength);
    Serial.print("Tag found: ");
    Serial.println(readID);

    keyDuino.buzz(20); //for NORMAL
    keyDuino.led(100); //for NANO

    // if NTAG21x enables r/w protection, uncomment the following line 
    // keyDuino.ntag21x_auth(password); 

    keyDuino.mifareultralight_ReadPage(3, buf);
    int capacity = buf[2] * 8;

    for (int i = 4; i < capacity / 4; i++) {
      keyDuino.mifareultralight_ReadPage(i, buf);
      keyDuino.PrintHexChar(buf, 4);
    }

    Serial.println("Done");
    Serial.println("----------------------------------------------------");
    Serial.println("press a key for new reading");
    while (!Serial.available());
    while (Serial.available()) Serial.read();
  }
  delay(1);
}
