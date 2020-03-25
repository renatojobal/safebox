# Safebox

Its an arduino code to control the access to a safebox using the MRC522 kit and a servomotor. :)

Program that get the UID from the card o key kepper at the kit RFID RC522. It compares
the previues values UID and give acces to a sasfebox, moving the servo motor that controls
the lock door.

It let you register some UIDS to let them the access.

For register the UIDS use the 'allowedUIDs' array

Uses MIFARE RFID card using RFID-RC522 reader
Uses MFRC522 - Library


## Connections

    - - - - - - - - - - - - - - - - - - - - - - - - - -
                MFRC522      Arduino       Arduino  
                Reader/PCD   Uno/101       Nano v3    
    Signal      Pin          Pin           Pin  
    - - - - - - - - - - - - - - - - - - - - - - - - - -
    RST/Reset   RST          9             D9   
    SPI SS      SDA(SS)      10            D10 
    SPI MOSI    MOSI         11 / ICSP-4   D11 
    SPI MISO    MISO         12 / ICSP-1   D12 
    SPI SCK     SCK          13 / ICSP-3   D13 

    - - - - - - - - - - - - - - - - - - - - - - - - - -
    Red Led                  3             D3
    Green Led                8             D8
    Servo motor              7             D7
    Button                   2             D2            


## Spanish:
Programa que obtiene el UID de la tarjeta o llavero del kit RFID RC522, compara con
valores previos obtenidos de UID y permite acceso a una caja fuerte, moviendo el servo
motor que controla la cerradura de la puerta.

Se pueden registrar varias UIDS a las que se permite el acceso.

Para registrar las UIDS usa el arreglo allowedUIDs


## Author: renatojobal