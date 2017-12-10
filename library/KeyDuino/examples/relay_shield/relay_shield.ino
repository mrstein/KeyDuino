/*
 You will need the relay shield to use this sketch. 
 The goal of this sketch is to activate the relay with KeyDuino and a NFC tag.
 Get the ID of your tag, put it in NFC_KEY below.
 When you scan the right tag with KeyDuino, the servo will activate the relay.
 Author: Mr Stein
 Revised by Raymond Borenstein - CITC-EuraRFID
 Compatible with KeyDuino 5.1
 Join http://keyduino.forumsactifs.com/ to ask your questions, suggest your ideas, and show your projects!
 */


#include "KeyDuino.h"

//Define relay pins
#define RELAY_1 7
#define RELAY_2 6
#define RELAY_3 5
#define RELAY_4 4

#define CHECK_ID true //Change to "true" if you want to check the good ID. If false, every tag will be accepted.
String NFC_KEY_1 = "XXXXXXXXXXXXXX"; //The tag n°1 UID you want to activate relays with.
String NFC_KEY_2 = "XXXXXXXXXXXXXX"; //The tag n°2 UID you want to activate relays with.


uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

KeyDuino keyDuino;

void changeRelaysState(int state);
boolean UintArrayCompare(uint8_t a[], uint8_t b[], int array_size);

void setup() {
  Serial.begin(115200);
  keyDuino.begin();

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  Serial.println("/!\\ Warning! /!\\");
  Serial.println("Before going on, please check that all relay switches are OFF on the board, then press a key.");
  Serial.println("Otherwise, you may risk to harm the relay shield ...");
  Serial.println("/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\/!\\");

  changeRelaysState(LOW);
  Serial.println("Waiting for a tag.");
}

void loop() {
  uint8_t success;
 success = keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);


  if (success) {
    String readID = keyDuino.convertUintToString(uid, uidLength);
    Serial.print("Tag found: ");
    Serial.println(readID);
    

   if (readID == NFC_KEY_1 || readID == NFC_KEY_2) {
      Serial.println("This is the right tag. Activating relays.");
      changeRelaysState(HIGH);
      delay(2000);
      changeRelaysState(LOW);
      delay(1000);
    } 
    else {
      Serial.println("This is not the right tag.");
      delay(500);
    }
  }
}

void changeRelaysState(int state) {
  digitalWrite(RELAY_1, state);
  digitalWrite(RELAY_2, state);
  digitalWrite(RELAY_3, state);
  digitalWrite(RELAY_4, state);
}

boolean UintArrayCompare(uint8_t a[], uint8_t b[], int array_size)
{
  for (int i = 0; i < array_size; i++)
    if (a[i] != b[i])
      return (false);
  return (true);
}

