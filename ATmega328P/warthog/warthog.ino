// Loosly based on original source from Low Power Lab
//
// Original copyright and license notice below
// **********************************************************************************
// Copyright Felix Rusu 2016, http://www.LowPowerLab.com/contact
// **********************************************************************************
// License
// **********************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 3 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE. See the GNU General Public        
// License for more details.                              
//                                                        
// Licence can be viewed at                               
// http://www.gnu.org/licenses/gpl-3.0.txt
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// **********************************************************************************
#include <SPI.h>           // Included with Arduino IDE (www.arduino.cc)
#include <LowPower.h>      // Get library from: https://github.com/lowpowerlab/lowpower
#include <RFM69.h>         // Get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     // Get it here: https://github.com/lowpowerlab/RFM69

#define NETWORKID     100  // The same on all nodes that talk to each other
#define RECEIVER      1    // Unique ID of the gateway/receiver
#define SENDER        2
#define NODEID        SENDER
#define FREQUENCY     RF69_433MHZ
#define ENCRYPTKEY    "warthog-dogbowl*" // Must be exactly the same 16 characters/bytes on all nodes!
#define ENABLE_ATC      //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI        -75
#define IS_RFM69HW


// ATmega382P pinout
//
//       RESET     |  1  28 |  19 LED6
//  RFM ENABLE  0  |  2  27 |  18 LED5
//          B1  1  |  3  26 |  17 LED4
// RFM G0/DIO0  2  |  4  25 |  16 LED3
//          J5  3  |  5  24 |  15 LED2
//   RFM RESET  4  |  6  23 |  14 LED1
//         VCC     |  7  22 |     GND
//         GND     |  8  21 |     AREF
//          J4     |  9  20 |     VCC
//          J3     | 10  19 |  13 SCK
//          L4  5  | 11  18 |  12 MISO
//          L3  6  | 12  17 |  11 MOSI
//          L2  7  | 13  16 |  10 RFM_SS
//          L1  8  | 14  15 |   9 J2

#define RFM_ENABLE  0
#define RFM_G0      2
#define RFM_RESET   4
#define RFM_SS     10

#define B1  1

#define J2  9
// #define J3  (no Arduino equivalent)
// #define J4  (no Arduino equivalent)
#define J5  3

#define L1   8
#define L2   7
#define L3   6
#define L4   5

#define LED1  14
#define LED2  15
#define LED3  16
#define LED4  17
#define LED5  18
#define LED6  19

#ifdef ENABLE_ATC
RFM69_ATC radio(RFM_SS, RFM_G0, true);
#else
RFM69 radio(RFM_SS, RFM_G0, true);
#endif

#define DECIMAL 10

LowPowerClass power; 

void initPin(byte pin, byte mode, byte level=0)
{
  pinMode(pin, mode);
  if (mode == OUTPUT)
  {
    digitalWrite(pin, level);
  }
}

void setup() {
  initPin(RFM_ENABLE, OUTPUT, LOW);
  initPin(RFM_G0,     OUTPUT, LOW);
  initPin(RFM_RESET,  OUTPUT, LOW);
  initPin(RFM_SS,     OUTPUT, LOW);

  initPin(B1, INPUT);
  initPin(J2, INPUT);
  initPin(J5, INPUT);

  initPin(L1, INPUT);
  initPin(L2, INPUT);
  initPin(L3, INPUT);
  initPin(L4, INPUT);

  initPin(LED1, OUTPUT, HIGH);
  initPin(LED2, OUTPUT, HIGH);
  initPin(LED3, OUTPUT, HIGH);
  initPin(LED4, OUTPUT, HIGH);
  initPin(LED5, OUTPUT, HIGH);
  initPin(LED6, OUTPUT, HIGH);
  
  blink(LED1, 50, 1);
  blink(LED2, 50, 1);
  blink(LED3, 50, 1);
  blink(LED4, 50, 1);
  blink(LED5, 50, 1);
  blink(LED6, 50, 1);
}


int packet_id = 0;
int last_level_sent = 255;

void loop() {

  bool debug = read(J2);
  if (debug) { blink(LED6, 50, 1); }
  
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  
  uint8_t level;
  if (!read(L4)) { level = 4; }
  else if (!read(L3)) { level = 3; } 
  else if (!read(L2)) { level = 2; }
  else if (!read(L1)) { level = 1; }
  else { level = 0; }
  
  if (debug) {
    switch (level) {
      case 4:
        digitalWrite(LED4, LOW);
      case 3:
        digitalWrite(LED3, LOW);
      case 2:
        digitalWrite(LED2, LOW);
      case 1:
        digitalWrite(LED1, LOW);
    }
  }
  
  if (level != last_level_sent) {
    digitalWrite(RFM_ENABLE, HIGH);
    delay(100);
    radio.initialize(FREQUENCY, NODEID, NETWORKID);
    radio.setHighPower();
    radio.encrypt(ENCRYPTKEY);
#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
#endif

    int len = 0;
    char text[100];
    len = sprintf(text, "Packet: %d Level: %d", packet_id++, level);
  
    if (debug) { blink(LED6, 50, 1); }
  
    bool acknowledged = radio.sendWithRetry(RECEIVER, text, len, 5, 255);
    if (acknowledged) {
      last_level_sent = level;
      if (debug) { blink(LED6, 50, 3); }
    }
    else {
      // The listener never acknowledges... :(
      last_level_sent = level; 
      if (debug) { blink(LED5, 50, 3); }
    }
    
    digitalWrite(RFM_ENABLE, LOW);
  }
  
  power.powerDown(SLEEP_8S, ADC_ON, BOD_ON);
}

void blink(byte PIN, int DELAY_MS, byte loops)
{
  for (byte i=0; i<loops; i++)
  {
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
  }
  digitalWrite(PIN, HIGH);
  delay(100);
}

bool read(byte pin)
{
  initPin(pin, INPUT_PULLUP);
  bool result = digitalRead(pin);
  initPin(pin, INPUT);
  return result;
}
