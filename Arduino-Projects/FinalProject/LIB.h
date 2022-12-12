
#include <EEPROM.h>     // We are going to read and write PICC's UIDs from/to EEPROM
#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>  // Library for Mifare RC522 Devices

/*
  Instead of a relaydoor maybe you want to use a servo
  Servos can lock and unlock door locks too
  There are examples out there.
*/

// #include <Servo.h>

/*
  For visualizing whats going on hardware
  we need some leds and
  to control door lock a relaydoor and a wipe button
  (or some other hardware)
  Used common anode led,digitalWriting HIGH turns OFF led
  Mind that if you are going to use common cathode led or
  just seperate leds, simply comment out #define COMMON_ANODE,
*/

#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH

#define LED_OFF LOW
#endif

#define redLed 7    // Set Led Pins
#define greenLed 6
#define blueLed 5

#define relaydoor 4     // Set relaydoor Pin
#define relaylamp 2

#define wipeB A0     // Button pin for WipeMode
#define btndoor A1   // Button pin for door
#define btnlamp A2   // Button pin for lamp


boolean match = false;          // initialize card match to false
boolean programMode = false;  // initialize programming mode to false
boolean replaceMaster = false;
bool lampval = 0;

int successRead;    // Variable integer to keep if we have Successful Read from Reader

byte storedCard[4];   // Stores an ID read from EEPROM
byte readCard[4];   // Stores scanned ID read from RFID Module
byte masterCard[4];   // Stores master card's ID read from EEPROM

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

/*
  We need to define MFRC522's pins and create instance
  Pin layout should be as follows (on Arduino Uno):
  MOSI: Pin 11 / ICSP-4
  MISO: Pin 12 / ICSP-1
  SCK : Pin 13 / ICSP-3
  SS : Pin 10 (Configurable)
  RST : Pin 9 (Configurable)
  look MFRC522 Library for
  other Arduinos' pin configuration
*/
