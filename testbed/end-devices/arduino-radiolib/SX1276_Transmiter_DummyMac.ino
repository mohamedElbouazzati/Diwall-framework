/*
   RadioLib SX127x Transmit Example

   This example transmits packets using SX1278 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>
#include "input_packet.h"
// SX1278 has the following connections:
// NSS pin:   10
// DIO0 pin:  2
// RESET pin: 9
// DIO1 pin:  3
SX1276 radio = new Module(10, 2, 9, 6);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin(868.3, 125.0, 12, 5, 0x34, 20, 8, 0);
  radio.setCurrentLimit(100);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
 ;
  }

}

void loop() {
  int i = 0;
  for (int j = 0; j < sizeof(Packet); ++j + i) {
  Serial.print(F("[SX1278] Transmitting packet ... "));  
    if (Packet[j] == 2) {
      char payload[Packet[j+1]] = {};
      int u = 0;
      for (int i = j + 2; i < Packet[j + 1] + 4 + j; ++i) {
        if (Packet[i] == 4) {
          Serial.println(payload);
          Serial.println(sizeof(payload));
          int state = radio.transmit(payload, sizeof(payload));
          if (state == RADIOLIB_ERR_NONE) {
            // the packet was successfully transmitted
            Serial.println(F(" success!"));
            // print measured data rate
            Serial.print(F("[SX1278] Datarate:\t"));
            Serial.print(radio.getDataRate());
            Serial.println(F(" bps"));
          } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
            // the supplied packet was longer than 256 bytes
            Serial.println(F("too long!"));
          } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
            // timeout occurred while transmitting packet
            Serial.println(F("timeout!"));
          } else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);
          }
          delay(2000);
          break;
        } else {
          payload[u] = Packet[i];
          u++;
        }
      }
    }
  }

}