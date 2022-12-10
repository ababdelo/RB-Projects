
/*************************************Including Libraries*******************************************/
#include<EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
/***************************************************************************************************/

/**************************************Definin' Pins********************************************/
#define RST_PIN 9
#define SS_PIN 10
#define BUZ 4
#define RDLED 5
#define GRLED 6
#define DOOR 7
/***************************************************************************************************/

/************************************Declaring used Variables***************************************/
String myTags[63]; // 64 UID ---> 8 x 64 = 512 byte of eeprom
char  sep = '_';
char *StoredUID;
int addrread = 0;
int addrwrite = 0;
String tagID = "";
boolean successRead = false;
boolean successStore = false;
boolean correctTag = false;
/***************************************************************************************************/

/***************************************Creating instances******************************************/
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 in PCF8574 or Set to 0x3F in PCF8574A
/***************************************************************************************************/

/************************************Declaring used Functiions*******************************************/

boolean getID();

int  ft_strstr(String str, String to_f);
void  ft_bzero(void *s, size_t n);

bool write2eeprom(int address, char* ID, char sep);
char *readfromeeprom(int address, char sep);

void OpenDoor();
void StandByMsg();
void access();
void denied();

/***************************************************************************************************/


/****************************************Setup Function*********************************************/
void setup()
{
  // Initiating
  SPI.begin();         // SPI bus
  mfrc522.PCD_Init();  //  MFRC522
  lcd.init();          // initialize lcd.
  lcd.begin(16, 2);    // LCD screen
  Serial.begin(9600);

  pinMode(BUZ, OUTPUT);
  pinMode(DOOR, OUTPUT);
  pinMode(RDLED, OUTPUT);
  pinMode(GRLED, OUTPUT);

  digitalWrite(DOOR, LOW);

  StandByMsg();
}

/***************************************************************************************************/

/*****************************************Loop Function*********************************************/
void loop()
{
  int index = 0;
  while (getID())
  {
    while (EEPROM.read(addrread++) != "") // Store UID of allowed users from eeprom to the array that'll be compored
      myTags[index] = readfromeeprom(addrread, sep);
    Serial.println(tagID);
    correctTag = false;
    // Checks whether the scanned tag is the master tag
    if (tagID == "CB5EFE19") {
      lcd.clear();
      lcd.print("Program mode:");
      lcd.setCursor(0, 1);
      lcd.print("Add/Remove Tag");
      while (!successRead) {
        successRead = getID();
        if (successRead == true) {
          for (int i = 0; i < 63; i++) {
            if (tagID == myTags[i]) {
              int ind = ft_strstr(myTags[i], tagID);
              ft_bzero(tagID[ind], strlen(tagID[ind]));
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("  Tag Removed!");
              StandByMsg();
              return;
            }
          }
          write2eeprom(addrwrite, tagID.c_str(), sep);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   Tag Added!");
          StandByMsg();
          return;
        }
      }
    } else {
      successRead = false;
      // Checks whether the scanned tag is authorized
      for (int i = 0; i < 63; i++) {
        if (tagID == myTags[i]) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Access Granted!");
          OpenDoor();
          StandByMsg();
          correctTag = true;
        }
      }

      if (correctTag == false) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Access Denied!");
        StandByMsg();
      }
    }
  }
}

/***************************************************************************************************/
