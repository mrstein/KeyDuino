#include "KeyDuino.h"

KeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;
uint8_t buf[4];
uint8_t password[4] = {0x12, 0x34, 0x56, 0x78}; //default readwrite password

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
    nfc.ntag21x_auth(password);
    
    keyDuino.mifareultralight_ReadPage(3, buf);
    int capacity = buf[2] * 8;

    uint8_t cfg_page_base = 0x29;   // NTAG213
    if (capacity == 0x3E) cfg_page_base = 0x83;       // NTAG215
    else if (capacity == 0x6D) cfg_page_base = 0xE3;       // NTAG216


    // PWD page, set new password
    keyDuino.mifareultralight_WritePage(cfg_page_base + 2, password);

    // disable r/w
    // | PROT | CFG_LCK | RFUI | NFC_CNT_EN | NFC_CNT_PWD_PROT | AUTHLIM (2:0) |
    buf[0] = (1 << 7) | 0x0;
    keyDuino.mifareultralight_WritePage(cfg_page_base + 1, buf);

    // protect pages started from AUTH0
    uint8_t auth0 = 0x10;
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = auth0;
    keyDuino.mifareultralight_WritePage(cfg_page_base, buf);


    Serial.println("Done");
    Serial.println("----------------------------------------------------");
    Serial.println("press a key for new reading");
    while (!Serial.available());
    while (Serial.available()) Serial.read();
  }
  delay(1);
}
