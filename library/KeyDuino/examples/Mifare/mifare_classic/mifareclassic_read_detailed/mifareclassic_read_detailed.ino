/*
  This sketch is made to be used with a Mifare Classic card, and serves as an example to read its sectors.
  It uses the MifareClassicKeyDuino class, which is an extension of KeyDuino class, easing the learning with Mifare Classic.

  Author: Pierre CHARLIER

  Compatible with KeyDuino 5.1

  Join http://keyduino.forumsactifs.com/ to ask your questions, suggest your ideas, and show your projects!
*/

#include <KeyDuino.h>


#define MIFARE_SIZE 16 // Defined data size: 16 if card is Mifare 1K, 64 if Mifare 4K
uint8_t data[MIFARE_SIZE][4][16] = {0};
boolean sector_key[MIFARE_SIZE];

/********************************** KEYS DEFINITION *********************************/
//Define here the keys of each sector of your Mifare Classic card, if you know them. Otherwise, only typical default keys will be used.
//Defined A-Keys Array
uint8_t definedKeysA[16][6] = {
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 }
};

// Defined B-Keys Array
uint8_t definedKeysB[16][6] = {
  { 0xA2, 0x2A, 0xE1, 0x29, 0xC0, 0x13 },
  { 0x49, 0xFA, 0xE4, 0xE3, 0x84, 0x9F },
  { 0x38, 0xFC, 0xF3, 0x30, 0x72, 0xE0 },
  { 0x8A, 0xD5, 0x51, 0x7B, 0x4B, 0x18 },
  { 0x50, 0x93, 0x59, 0xF1, 0x31, 0xB1 },
  { 0x6C, 0x78, 0x92, 0x8E, 0x13, 0x17 },
  { 0xAA, 0x07, 0x20, 0x01, 0x87, 0x38 },
  { 0xA6, 0xCA, 0xC2, 0x88, 0x64, 0x12 },
  { 0x62, 0xD0, 0xC4, 0x24, 0xED, 0x8E },
  { 0xE6, 0x4A, 0x98, 0x6A, 0x5D, 0x94 },
  { 0x8F, 0xA1, 0xD6, 0x01, 0xD0, 0xA2 },
  { 0x89, 0x34, 0x73, 0x50, 0xBD, 0x36 },
  { 0x66, 0xD2, 0xB7, 0xDC, 0x39, 0xEF },
  { 0x6B, 0xC1, 0xE1, 0xAE, 0x54, 0x7D },
  { 0x22, 0x72, 0x9A, 0x9B, 0xD4, 0x0F },
  { 0x48, 0x45, 0x58, 0x41, 0x43, 0x54 }
};


MifareClassicKeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

void setup(void) {
  Serial.begin(115200);
  keyDuino.begin();
  keyDuino.SAMConfig();
  Serial.println("Waiting for a Mifare Classic tag to read.");
}



/****************************** detect a card *********************
   this function will loop while until it detect a mifare classic
*/
void detectCard() {
  uint8_t success;
  while (1) {
    success = keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   //Try to read card UID
    if (success) {
      if (uidLength == 4) {
        Serial.print("Mifare Classic identified: UID: ");
        keyDuino.PrintHex(uid, uidLength);
        break;
      }
      else {
        Serial.println("Detected tag is not Mifare Classic.");
        delay(500);
      }
    }
  }
}

/****************************** Read all blocks *********************
   this function try each defined keys and add the data in the data array
   the data array is defined like that: data[sector][block][16]
*/
void readData() {
  Serial.print("reading, please wait");

  for (int sector = 0 ; sector < MIFARE_SIZE ; sector++) { // 16 sectors if card is Mifare 1K, 64 if Mifare 4K
    if (keyDuino.mifareclassic_AuthenticateSectorDefaultKeys(sector) || keyDuino.authenticateDefinedKey(definedKeysA[sector], MIFARE_KEY_A, sector) || keyDuino.authenticateDefinedKey(definedKeysB[sector], MIFARE_KEY_B, sector)) { // try sector key
      sector_key[sector] = true;
      for (int block = 0 ; block < 4; block++)  keyDuino.mifareclassic_ReadDataBlock(sector * 4 + block, data[sector][block]);
    }
    else {
      Serial.print("Failed authentication on sector ");
      Serial.println(sector);
      sector_key[sector] = false;
      for (int block = 0 ; block < 4; block++)  for (int i = 0 ; i < 16; i++) data[sector][block][i] = 0; //keys are not goods
    }
    Serial.print(".");
  }
  Serial.println("");
}

/****************************** Print all block *********************
   this function will serial print all reading blocks. If one secteur is filled with only X, it's because you don't have the correct key
*/
void printData() {
  for (int sector = 0 ; sector < MIFARE_SIZE ; sector++) {
    Serial.print("---------------------\n Sector ");  Serial.println(sector);
    if (sector_key[sector] == true)  for (int block = 0 ; block < 4; block++) keyDuino.PrintHex(data[sector][block], 16);
    else for (int block = 0 ; block < 4; block++)  Serial.println("xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx");  //wrong key
  }
}

void loop(void) {
  detectCard();  //first we detect a card
  readData();    //then we try to read all datas with the defined keys
  printData();   //and print all datas

  Serial.println("----------------------------------------------------");
  Serial.println("press a key for new reading");
  while (!Serial.available());
  while (Serial.available()) Serial.read();
}



