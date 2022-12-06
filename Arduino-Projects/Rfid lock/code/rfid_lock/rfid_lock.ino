#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5

#define SS_PIN 10
#define RD_LED 5
#define GR_LED 6
#define DRLOCK 7

String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  // Initiating
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  Serial.begin(9600); // Initializing Serial for communication
  pinMode(RD_LED, OUTPUT); // Setting the Mode for pin RD_LED
  pinMode(GR_LED, OUTPUT);// Setting the Mode for pin GR_LED
  pinMode(DRLOCK, OUTPUT);// Setting the Mode for pin DR_LOCK
  digitalWrite(DRLOCK, LOW);
  Serial.print("\nAccess Control");
}

void loop()
{
  //Wait until new tag is available
  while (getID())
  {
    Serial.print("\nScan Your Card --->");
    Serial.print(" ID : ");
    Serial.print(tagID);
    if (tagID == "CB5EFE19" || tagID == "B4CC13CE" || tagID == "E4ADBCFE" ||
        tagID == "541E1DFF" || tagID == "4569CE" || tagID == "947FDCCE")
      // Allowed UID-TAG, You can add whoever u want by knowing his id tag
    {
      digitalWrite(GR_LED, HIGH);
      digitalWrite(RD_LED, LOW);
      Serial.print("\n Access Granted!");
      opendoor();
      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
    }
    else
    {
      digitalWrite(GR_LED, LOW);
      digitalWrite(RD_LED, HIGH);
      Serial.print("\nAccess Denied!");
    }
    delay(500);
    digitalWrite(GR_LED, LOW);
    digitalWrite(RD_LED, LOW);
  }
}

//Read new tag if available
boolean getID()
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}

void opendoor()
{
  digitalWrite(DRLOCK, HIGH);
  delay(2500);
  digitalWrite(DRLOCK, LOW);
}
