// rf69 demo tx rx.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_server.
// Demonstrates the use of AES encryption, setting the frequency and modem 
// configuration

#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 434.0

#define RFM69_INT     3  // 
#define RFM69_CS      10  //
#define RFM69_RST     2  // "A"
#define RED           9
#define GREEN         8

#define F_CPU 1000000

// Singleton instance of the radio driver
// RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission

void setup() 
{
  pinMode(RED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);


  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    while (1) { blink(RED, 50, 1); }
  }
  // RFM69 radio init OK!
  blink(GREEN, 50, 10);
}
//    blink(LED, 50, 10);
//  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
//  // No encryption
//  if (!rf69.setFrequency(RF69_FREQ)) {  
//    while(1) { blink(LED, 50, 1); }
//    // setFrequency failed"
//  }
//
//  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
//  // ishighpowermodule flag set like this:
//  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW
//
//  // The encryption key has to be the same as the one in the server
//  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
//                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//  rf69.setEncryptionKey(key);
//  
//  blink(LED, 50, 10); 
//}
//
//
//
void loop() {
  while(1)
  {
    blink(GREEN, 1000, 1);
  }
}
//  char radiopacket[20] = "Hello World #";
//  itoa(packetnum++, radiopacket+13, 10);
//  
//  // Send a message!
//  rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
//  rf69.waitPacketSent();
//
//  // Now wait for a reply
//  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
//  uint8_t len = sizeof(buf);
//
//  if (rf69.waitAvailableTimeout(500))  { 
//    // Should be a reply message for us now   
//    if (rf69.recv(buf, &len)) {
//      blink(LED, 50, 3); //blink LED 3 times, 50ms between blinks
//    } else {
//      // Receive failed
//    }
//  } else {
//    // No reply, is another RFM69 listening?
//  }
//}

void blink(byte PIN, int DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}
