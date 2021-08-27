// Loosly based on original source from Low Power Lab
//
// Pair this transmitter test with python/rx-test.py
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
#include <SPI.h>           //included with Arduino IDE (www.arduino.cc)
#include <LowPower.h>      //get library from: https://github.com/lowpowerlab/lowpower
#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://github.com/lowpowerlab/RFM69

//****************************************************************************************************************
//**** IMPORTANT RADIO SETTINGS - YOU MUST CHANGE/CONFIGURE TO MATCH YOUR HARDWARE TRANSCEIVER CONFIGURATION! ****
//****************************************************************************************************************
#define NETWORKID     100  //the same on all nodes that talk to each other
#define RECEIVER      1    //unique ID of the gateway/receiver
#define SENDER        2
#define NODEID        SENDER  //change to "SENDER" if this is the sender node
#define FREQUENCY     RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
//*****************************************************************************************************************************
#define ENABLE_ATC      //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI        -75
//*********************************************************************************************

#define GREEN       8 // GREEN LED on the SENDER
#define RED         9 // RED LED on the SENDER
#define RESET       4 
#define ENABLE      0 

#ifdef ENABLE_ATC
RFM69_ATC radio;
#else
RFM69 radio;
#endif

void setup() {
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(RESET, LOW);
  digitalWrite(ENABLE, LOW);
  
  blink(GREEN, 50, 5);
  blink(RED, 50, 5);
  blink(GREEN, 50, 5);
}

int count = 0;
void loop() {
  int len = 0;
  char text[20] = "Packet #";
  len = strlen(text);
  itoa(count++, text+len, 10);
  len = strlen(text);

  digitalWrite(ENABLE, HIGH);
  delay(100);
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRYPTKEY);
#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
#endif
  
  blink(RED, 50, 1);
  if (radio.sendWithRetry(RECEIVER, text, len)) { //target node Id, message as string or byte array, message length
    blink(GREEN, 50, 3);
  }
  else {
    blink(RED, 50, 3);
  }
  
  digitalWrite(ENABLE, LOW);
  delay(5000);
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
