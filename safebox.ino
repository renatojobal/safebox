/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/
/*
  EN
  Program that get the UID from the card o key kepper at the kit RFID RC522. It compares
  the previues values UID and give acces to a sasfebox, moving the servo motor that controls
  the lock door.

  It let you register some UIDS to let them the access.

*/
/*
  ES
  Programa que obtiene el UID de la tarjeta o llavero del kit RFID RC522, compara con
  valores previos obtenidos de UID y permite acceso a una caja fuerte, moviendo el servo
  motor que controla la cerradura de la puerta.

  Se pueden registrar varias UIDS a las que se permite el acceso.

*/

#include <SPI.h>     // Include library bus SPI
#include <MFRC522.h> // Include library for MFRC522
#include <Servo.h>   // Include library for servomotor

#define RST_PIN 9 // Constant of reset pin
#define SS_PIN 10 // Constant of slave select pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create the object mfrc522

/*
Keys
- 09 7E D7 20 (With the house keys)
- 56 DB BD F7


Cards
- 42 37 F4 1F (With the plastic)
- 42 78 4A 34
- DE 96 8B 21

*/

// byte Usuario1[4] = {0x90, 0x0E, 0xE4, 0xA4}; // UID de tarjeta leido en programa 1
// byte Usuario2[4] = {0x06, 0x76, 0x25, 0xD9}; // UID de llavero leido en programa 1

byte targetUID[4]; // for saving the result of the reader

byte allowedUIDs[2][4] = {
    {0x09, 0x7E, 0xD7, 0x20},
    {0x42, 0x37, 0xF4, 0x1F}};

int greenLedPin = 8;
int redLedPin = 3;
Servo servomotor;
int servoPin = 7;
int minPulse = 700;
int maxPulse = 2300;
int buttonPin = 2;

void setup()
{
  Serial.begin(9600); // Initialice the serial port
  SPI.begin();        // initialice bus SPI
  mfrc522.PCD_Init(); // initialice reder module

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, INPUT);
  servomotor.attach(servoPin);
  closeDoor();
  Serial.println("Ready\n"); // Ok message
}

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
  { // bucle recorre de a un byte por vez el UID
    if (mfrc522.uid.uidByte[i] < 0x10)
    {
      Serial.print(" 0");
    }
    else
    {
      Serial.print(" ");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX); // imprime el byte del UID leido en hexadecimal
    targetUID[i] = mfrc522.uid.uidByte[i];     // almacena en array el byte del UID leido
  }
  Serial.print("\t"); //

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

boolean areSameUIDS(byte target1[], byte target2[])
{
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (target1[i] != target2[i])
      return (false);
  }
  return (true);
}

boolean canRead()
{

  // Control sentences
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
handleCorrectUID

The digital pins allowed are: 8, 7, 6, 5, 4, 3, 2
The analog  pins allowed are: 1, 2, 3, 4, 5, 6, 7



*/

void handleCorrectUID()
{
  digitalWrite(greenLedPin, HIGH);
  openDoor();
  delay(2000);
  digitalWrite(greenLedPin, LOW);
}

/*
handleinCorrectUID


*/
void handleIncorrectUID()
{
  digitalWrite(redLedPin, HIGH);
  delay(2000);
  digitalWrite(redLedPin, LOW);
}

void openDoor()
{
  servomotor.write(180);
  delay(200);
}
void closeDoor()
{
  servomotor.write(0);
  digitalWrite(redLedPin, HIGH);
  delay(2000);
  digitalWrite(redLedPin, LOW);
}

boolean buttonPressed()
{
  Serial.print(digitalRead(buttonPin));
  return digitalRead(buttonPin);
}
