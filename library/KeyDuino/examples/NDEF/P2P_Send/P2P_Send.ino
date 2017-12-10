/*
  This sketch allows a simple communication between KeyDuino and a NFC smartphone through NDEF.
  Author: Pierre CHARLIER
  Compatible with KeyDuino 5.1
  Join http://keyduino.forumsactifs.com/ to ask your questions, suggest your ideas, and show your projects!
*/

#include <KeyDuino.h>
#include "snep.h"
#include "NdefMessage.h"

KeyDuino keyDuino;
SNEP nfc(keyDuino);
uint8_t ndefBuf[128];

String URI = "http://github.com/CITCEuraRFID/KeyDuino"; //Website URL
//String URI = "tel:0600000000"; //Phone number
//String URI = "mailto:keyduino@outlook.com"; //Email address

void setup() {
  Serial.begin(115200);
  Serial.println("Peer to peer");
}

void loop() {
  Serial.println("Send a message to Peer");

  NdefMessage message = NdefMessage();
  message.addUriRecord(URI);


  int messageSize = message.getEncodedSize();
  if (messageSize > sizeof(ndefBuf)) {
    Serial.println("ndefBuf is too small");
    while (1) {
    }
  }

  message.encode(ndefBuf);
  if (0 >= nfc.write(ndefBuf, messageSize)) {
    Serial.println("Failed");
  } else {
    Serial.println("Success");
  }

  delay(3000);
}
