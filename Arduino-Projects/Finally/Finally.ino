
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
char* myTags[31];
char *StoredUID; // The string where all uid wil be stored in
int tagsCount = 0;
int address = 10;
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

size_t ft_strlen(char *str);
char  *ft_strdup(char *s1);
char  *ft_substr(char const *s, unsigned int start, size_t len);
int  wrd_cnt(char const *s, char del);
char  **ft_free(char **str, int x);
char  **ft_split(char const *s, char c);

void Store2Eeprom(int *address, char* ID);
char *ReadfromEeprom(int *address);

void OpenDoor();
void StandByMsg();
void access();
void denied();

/***************************************************************************************************/


/****************************************Setup Function*********************************************/
void setup() {
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
void loop() {

  while (getID()) {
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
          for (int i = 0; i < 31; i++) {
            if (tagID == myTags[i]) {
              myTags[i] = "";
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("  Tag Removed!");
              StandByMsg();
              return;
            }
          }
          myTags[tagsCount] = tagID.c_str();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   Tag Added!");
          StandByMsg();
          tagsCount++;
          return;
        }
      }
    } else {
      successRead = false;
      // Checks whether the scanned tag is authorized
      for (int i = 0; i < 31; i++) {
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
