/*
 * Safebox
 * Author: renatojobal (https://github.com/renatojobal)
 * 
 * 
 * Program that get the UID from the card o key kepper at the kit RFID RC522. It compares
 * the previues values UID and give acces to a sasfebox, moving the servo motor that controls
 * the lock door.
 *
 * It let you register some UIDS to let them the access.
 *
 * For register the UIDS use the 'allowedUIDs' array

 * 
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * 
 * 
 * Connections
 * 
 * ---------------------------------------------------
 *             MFRC522      Arduino       Arduino  
 *             Reader/PCD   Uno/101       Nano v3    
 * Signal      Pin          Pin           Pin  
 * ---------------------------------------------------
 * RST/Reset   RST          9             D9   
 * SPI SS      SDA(SS)      10            D10 
 * SPI MOSI    MOSI         11 / ICSP-4   D11 
 * SPI MISO    MISO         12 / ICSP-1   D12 
 * SPI SCK     SCK          13 / ICSP-3   D13 
 * 
 * 
 *
 * ---------------------------------------------------
 * Red Led                  3             D3
 * Green Led                8             D8
 * Servo motor              7             D7
 * Button                   2             D2            


 

*/

/*
  * Spanish 
  * Programa que obtiene el UID de la tarjeta o llavero del kit RFID RC522, compara con
  * valores previos obtenidos de UID y permite acceso a una caja fuerte, moviendo el servo
  * motor que controla la cerradura de la puerta.

  * Se pueden registrar varias UIDS a las que se permite el acceso.

  * Para registrar las UIDS usa el arreglo allowedUIDs

*/

#include <SPI.h>     // Include library bus SPI
#include <MFRC522.h> // Include library for MFRC522
#include <Servo.h>   // Include library for servomotor

#define RST_PIN 9 // Constant of reset pin
#define SS_PIN 10 // Constant of slave select pin

// Put here your UIDS  of the Cards.
// Remember to change the numbere of de first '[]', for example if you have only one UID, you
// should put like this 'byte allowedUIDs[1][4] = { {...} }'
byte allowedUIDs[2][4] = {
    {0x09, 0x7E, 0xD7, 0x20},
    {0x42, 0x37, 0xF4, 0x1F}};

byte targetUID[4]; // for saving the result of the reader

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create the object mfrc522

// Ataching pins
int greenLedPin = 8;
int redLedPin = 3;
int servoPin = 7;
int minPulse = 700;
int maxPulse = 2300;
int buttonPin = 2;

Servo servomotor; // Creating a instance of Servo

/* 
  This function executes only one time, when the Arduino is connected to power.
*/
void setup()
{
  Serial.begin(9600); // Initialice the serial port
  SPI.begin();        // initialice bus SPI
  mfrc522.PCD_Init(); // initialice reder module

  // Ataching some
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, INPUT);

  servomotor.attach(servoPin);
  closeDoor();

  Serial.println("Ready\n"); // Ok message
}

/* 
  Principal function. It will be executed forever. After the setup function.
*/
void loop()
{
  delay(200);

  if (buttonPressed())
  {
    closeDoor();
  }

  // Control sentences
  if (canRead())
  { // if its possible read a UID
    Serial.print("\nUID Found\n");
  }
  else
  {
    return;
  }

  // Getting the read and saving it on targetUID, and printing it
  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (mfrc522.uid.uidByte[i] < 0x10)
    {
      Serial.print(" 0");
    }
    else
    {
      Serial.print(" ");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    targetUID[i] = mfrc522.uid.uidByte[i];
  }
  Serial.print("\t");

  // Comparing the targetUID with the allowedUIDs
  if (isAnAlloweUID())
  {
    Serial.print("\nAllowed UID\n");
    /* ToDo something if it is allowed ... */
    handleCorrectUID();
  }
  else
  {
    Serial.print("\nUID not allowed\n");
    handleIncorrectUID();
  }

  mfrc522.PICC_HaltA(); // stop the comunication
}

/*
  It returns true when the read UID is in the 'allowedUIDs' array.
*/
boolean isAnAlloweUID()
{ // Comparing the targetUID wit the allowedUIDs

  // Comparing each UID allowedUID, so we need two fors, because it is
  // a matriz

  for (byte i = 0; i < sizeof(allowedUIDs); i++)
  {
    /*Compare each UID allowed*/
    if (areSameUIDS(targetUID, allowedUIDs[i]))
    {
      return true;
    }
  }
  return (false);
}

/*
  Returns true when the args passed are equals.
*/
boolean areSameUIDS(byte target1[], byte target2[])
{
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (target1[i] != target2[i])
      return (false);
  }
  return (true);
}

/*
  Control sentences
*/
boolean canRead()
{

  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }
  return true;
}

/*
 This function is executed when the UID of the card read its allowe.
 We open the door.
*/
void handleCorrectUID()
{
  digitalWrite(greenLedPin, HIGH);
  openDoor();
  delay(2000);
  digitalWrite(greenLedPin, LOW);
}

/*
  This function is executed when the UID of the car read is not allowed.
  We turn on the red led to show the user that we read the UID but the it is not in the allowed UID list. 
*/
void handleIncorrectUID()
{

  digitalWrite(redLedPin, HIGH);
  delay(2000);
  digitalWrite(redLedPin, LOW);
}

/*
  To open the door with the servo motor
*/
void openDoor()
{
  servomotor.write(180);
  delay(200);
}

/*
  To close the door with the servo motor
*/
void closeDoor()
{
  servomotor.write(0);
  digitalWrite(redLedPin, HIGH);
  delay(2000);
  digitalWrite(redLedPin, LOW);
}

/*
  Listenner of the button
*/
boolean buttonPressed()
{
  Serial.print(digitalRead(buttonPin));
  return digitalRead(buttonPin);
}
