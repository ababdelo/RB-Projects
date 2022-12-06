//Make sure to get the right libraries

//Include the library used in th code
#include <Keypad.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Define the pins and the values

int Btn_L = A1;
int Btn_D = A0;

int lamp = A2;
int door = A3;

int buzzer = 12;
int rdled = 11;
int gnled = 10;

bool lampval /* ,settime*/;

const byte ROWS = 4;         //number of rows on the keypad
const byte COLS = 4;         //number of columns on the keypad

char keymap[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char keypressed;                 //Where the keys are stored it changes very often
char code[4] = {'1', '9', '7', '3'}; //The default code.

char code_buff1[sizeof(code)];  //Where the new key is stored
char code_buff2[sizeof(code)];  //Where the new key is stored again so it's compared to the previous one

unsigned int a = 0, i = 0, s = 0, j = 0, e = 5, d = 15, x = 0, y = 0;
//    j: refaire to the index of column in LCD_I2C
//    a: to store variables of function GetCode
//    e: attempt counnter
//    d: timer

//                  R1, R2, R3, R4
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
//                  C1, C2, C3, C4
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 in PCF8574 or Set to 0x3F in PCF8574A

byte StandBy[] = {B00111, B01110, B11100, B11000, B11000, B11100, B01110, B00111};

byte O_Lock[] = {B01110, B10001, B10001, B00001, B11111, B11011, B11011, B11111};

byte C_Lock[] = {B01110, B10001, B10001, B10001, B11111, B11011, B11011, B11111};

void setup()  // the setup function runs once when you press reset or power the board
{
  // initialize digital pin.
  pinMode(Btn_L, INPUT_PULLUP);
  pinMode(Btn_D, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(door, OUTPUT);

  pinMode(rdled, OUTPUT);
  pinMode(gnled, OUTPUT);

  lcd.init();// initialize lcd.
  lcd.begin (16, 2);
  lcd.backlight();
  lcd.print("    Stand_By    ");      //What's written on the LCD you can change

  digitalWrite(lamp, HIGH);///////
  lampval = 1;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // (It's done only once  ? ? ?  )
  for (i = 0 ; i < sizeof(code); i++) //When you upload the code the first time keep it commented
  {
    EEPROM.get(i, code[i]);             //Upload the code and change it to store it in the EEPROM
  }                                  //Then uncomment this for loop and reupload the code
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void loop()  // the loop function runs over and over again forever
{
  if (e == 0)
  {
    Timer();
  }
  else
  {
    d = 15;
    keypressed = myKeypad.getKey();               //Constantly waiting for a key to be pressed
    if (keypressed == '*')
    {
      // * to start entering the code
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Enter Code   ");            //Message to show
      GetCode();           //Getting code function
      if (a == sizeof(code))        //The GetCode function assign a value to a (it's correct when it has the size of the code array)
        OpenDoor();                   //Open lock function if code is correct
      else {
        lcd.clear();
        lcd.print("      Wrong     ");          //Message to print when the code is wrong
        denied();
        attempt();
      }
      delay(1000);
      lcd.clear();
      lcd.print("    Stand_By    ");             //Return to standby mode it's the message do display when waiting
    }
  }
  if (keypressed == 'C') {                //To change the code it calls the changecode function
    ChangeCode();
    lcd.clear();
    lcd.print("    Stand_By    ");                 //When done it returns to main mode
  }

  /////////////////////////////////////The Lamp Button Inside The Room//////////////////////////////////////////////////////
  if (digitalRead(Btn_L) == LOW)
  {
    while (digitalRead(Btn_L) == LOW) {
      delay(50);
    }
    lampval = ! lampval;
    digitalWrite(lamp, lampval);
  }
  ///////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////The Door Button Inside The Room////////////////////////
  if (digitalRead(Btn_D) == LOW) {
    OpenDoor();
  }
  ///////////////////////////////////////////////////////////////////////////////////////////
}

//###############################
void GetCode()
{
  //Getting code sequence
  i = 0;
  a = 0;
  j = 6;                    // You can change it value of j
  y = i;
  x = 4;
  while (keypressed != '#')
  {
    //The user press # to confirm the code otherwise he can keep typing
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != '#' ) {     //If the char typed isn't C and neither "nothing"
      bip();
      if (y < x)  // We do not want to print more than the size of the code[]
      {
        lcd.setCursor(j, 1);                            //This to write "*" on the LCD whenever a key is pressed it's position is controlled by j
        lcd.print("*");
        j++;
        x--;
      }
      if (keypressed == code[i] && i < sizeof(code)) {       //if the char typed is correct a and i increments to verify the next caracter
        a++;                                              //Now I think maybe I should have use only a or i ... too lazy to test it -_-'
        i++;
      }
      else
        a--;                                               //if the character typed is wrong a decrements and cannot equal the size of code []
    }
  }
  keypressed = NO_KEY;
}
//###############################

//###############################
void ChangeCode()
{
  //Change code sequence
  lcd.clear();
  lcd.print("  Changing Code ");
  delay(1000);
  lcd.clear();
  lcd.print(" Enter Old Code ");
  GetCode();                      //verify the old code first so you can change it
  if (a == sizeof(code))
  {
    //again verifying the a value
    lcd.clear();
    lcd.print("  Changing Code ");
    GetNewCode1();            //Get the new code
    GetNewCode2();            //Get the new code again to confirm it
    s = 0;
    for (i = 0 ; i < sizeof(code) ; i++)
    {
      //Compare codes in array 1 and array 2 from two previous functions
      if (code_buff1[i] == code_buff2[i])
        s++;                                //again this how we verifiy, increment s whenever codes are matching
    }
    if (s == sizeof(code))
    {
      //Correct is always the size of the array
      for (i = 0 ; i < sizeof(code) ; i++)
      {
        code[i] = code_buff2[i];       //the code array now receives the new code
        EEPROM.put(i, code[i]);        //And stores it in the EEPROM
      }
      lcd.clear();
      lcd.print("  Code Changed  ");
      delay(2000);
      e = 3;
    }
    else
    {
      //In case the new codes aren't matching
      lcd.clear();
      lcd.print("  Codes Are Not ");
      lcd.setCursor(0, 1);
      lcd.print("   Matching !!  ");
      denied();
      delay(2000);
    }
  }
  else
  { //In case the old code is wrong you can't change it
    lcd.clear();
    lcd.print("      Wrong     ");
    attempt();
    denied();
    delay(2000);
  }
}
//###############################

//###############################
void GetNewCode1()
{
  i = 0;
  j = 6;
  x = 4;
  lcd.clear();
  lcd.print(" Enter New Code ");   //tell the user to enter the new code and press C
  lcd.setCursor(0, 1);
  lcd.print("   And Press C  ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("   And Press C  ");     //Press C keep showing while the top row print ***
  while (keypressed != 'C') {          //C to confirm and quits the loop
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'C' ) {
      bip();
      if (y < x)  // We do not want to print more than the size of the code[]
      {
        lcd.setCursor(j, 0);                            //This to write "*" on the LCD whenever a key is pressed it's position is controlled by j
        lcd.print("*");
        x--;
      }          //On the new code you can show * as I did or change it to keypressed to show the keys
      code_buff1[i] = keypressed;   //Store caracters in the array
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}
//###############################

//###############################
void GetNewCode2()
{
  //This is exactly like the GetNewCode1 function but this time the code is stored in another array
  i = 0;
  j = 6;
  x = 4;

  lcd.clear();
  lcd.print("  Confirm Code  ");
  lcd.setCursor(0, 1);
  lcd.print("   And Press C  ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("   And Press C  ");
  while (keypressed != 'C')
  {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'C' )
    {
      bip();
      if (y < x)  // We do not want to print more than the size of the code[]
      {
        lcd.setCursor(j, 0);                            //This to write "*" on the LCD whenever a key is pressed it's position is controlled by j
        lcd.print("*");
        x--;
      }
      code_buff2[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}
//###############################

//###############################
void OpenDoor()
{
  //Lock opening function open for 3s
  lcd.clear();
  lcd.print("     Welcome    ");       //With a message printed
  e = 5;
  digitalWrite(door, HIGH);
  acces();
  delay(2600) ;
  digitalWrite(door, 0);
  lcd.clear();
  lcd.print("    Stand_By    ");
}
//###############################

//###############################
void bip()
{
  digitalWrite(buzzer, HIGH);
  delay(50) ;
  digitalWrite(buzzer, LOW);
}
//###############################

//###############################
void acces()
{
  digitalWrite(buzzer, HIGH);
  digitalWrite(gnled, HIGH);
  delay(100) ;
  digitalWrite(buzzer, LOW);
  digitalWrite(gnled, LOW);
  delay(100) ;
  digitalWrite(buzzer, HIGH);
  digitalWrite(gnled, HIGH);
  delay(200) ;
  digitalWrite(buzzer, LOW);
  digitalWrite(gnled, LOW);
}
//################################
void denied()
{
  digitalWrite(buzzer, HIGH);
  digitalWrite(rdled, HIGH);
  delay(700) ;
  digitalWrite(buzzer, LOW);
  digitalWrite(rdled, LOW);
}
//###############################

//###############################
void Timer()
{
  for (int o = 0; o < 15; o++)
  {
    d--;
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Try again in ");
    lcd.setCursor(13, 0);
    lcd.print(d / 10);
    lcd.setCursor(14, 0);
    lcd.print(d % 10);
  }
  if (d == 0)
  {
    lcd.clear();
    lcd.print("    Stand_By    ");             //Return to standby mode it's the message do display when waiting
    e = 5;
  }
}
//###############################

//###############################
void attempt()
{
  e--;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("attempt left");         //Message to print when the code is wrong to show the attempt left
  lcd.setCursor(1, 0);
  lcd.print(e);
}
//###############################
