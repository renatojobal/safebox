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

String targetUID; // for saving the result of the reader

String allowedUIDs[2] = {
    "097ED720",
    "4237F41F"};

/*




*/
void setup()
{
  Serial.begin(9600);        // Initialice the serial port
  SPI.begin();               // initialice bus SPI
  mfrc522.PCD_Init();        // initialice reder module
  Serial.println("Ready\n"); // Ok message
}

/*




*/
void loop()
{

  /*




  */
  // Control sentences
  if (canRead())
  { // if its possible read a UID
    Serial.print("UID Found\n");
  }
  else
  {
    return;
  }

  /*




  */
  // Getting the read and saving it on targetUID, and printing it
  targetUID = read();
  Serial.print("UID: " + targetUID);
  Serial.println(targetUID);

  /*




  */
  // Comparing the targetUID with the allowedUIDs
  if (isAnAlloweUID())
  {
    Serial.println("\nAllowed UID");
  }
  else
  {
    Serial.println("\nUID not allowed");
  }

  mfrc522.PICC_HaltA(); // stop the comunication
}

/*




*/
boolean isAnAlloweUID()
{ // Comparing the targetUID wit the allowedUIDs

  // Comparing each UID allowedUID, so we need two fors, because it is
  // a matriz

  for (byte i = 0; i < sizeof(allowedUIDs); i++)
  {
    if (areSameUIDs(targetUID, allowedUIDs[i]))
    {
      return true;
    }
  }

  return false;
}

/*




*/
boolean areSameUIDs(String target1, String target2) // funcion compareUIDs
{
  if (target1 == target2){
  return (true);}
}

/*





*/
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



*/
String read(){
  // Getting the read and saving it on targetUID, and printing it

  String result = "";

  for (byte i = 0; i < mfrc522.uid.size; i++)
  { 
    if (mfrc522.uid.uidByte[i] < 0x10)
    {
      Serial.print("0");
      result += "0";
    }else{
      Serial.print(" ");
    }
    sprintf
    result += "%x", mfrc522.uid.uidByte[i];    
  }
  return result;

}
