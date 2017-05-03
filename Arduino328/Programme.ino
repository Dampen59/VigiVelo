/*************************************
    DECLARATIONS POUR MODULE RFID
 *************************************/
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <RFID_eBlocks.h>
#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <LiquidCrystal.h>

/* déclaration des entrées/sorties de
   la liaison série avec le module RFID */
const int rxRFID = 2;
const int txRFID = 3;
const int ctsRFID = 4;
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x1D, 0x1A}; // Adresse mac du shield
byte ip[] = {172, 16, 102, 75}; // Ip de l'arduino
byte server[] = {172, 16, 102, 14}; // IP Serveur de cartes
String CardHTTPStatus;
char inString[32]; // Pour le port serie
int stringPos = 0; // compteur
boolean startRead = false; // bool
EthernetClient client;

// Déclaration module SIM900
int8_t answer;
int onModulePin = 6;
char aux_string[30];
char phone_number[] = "+33000000000";

// Déclaration LEDS
int LedRouge = A2;
int LedVerte = A3;

/* Déclaration de la liaison série logicielle */
SoftwareSerial mySerial(rxRFID, txRFID);
/* Déclaration de l'objet RFID */
RFID myRFID(ICODE, ctsRFID); // ICODE protocol, RX, TX et CTS en pin 2, 3, 4
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
/*************************************
    FIN DECLARATIONS POUR MODULE RFID
 *************************************/

byte statusByte;  // Status de retour
byte UID;// Donnée UID
byte UIDArray[8];
byte oct1;
byte oct2;
byte oct3;
byte oct4;
byte oct5;
byte oct6;
byte oct7;
byte oct8;
boolean nightMode = false;


// Déclaration PIN LOQUET
int readOpto = 7;
int cmdOptoLock = 6;
int cmdOptoUnlock = 5;
int openDoor = 8;
int MoteurOuverture = A0;
int MoteurFermeture = A1;

void setup() {
  pinMode(cmdOptoUnlock, OUTPUT);
  pinMode(cmdOptoLock, OUTPUT);
  pinMode(readOpto, INPUT);
  pinMode(openDoor, INPUT);
  pinMode(MoteurOuverture, OUTPUT);
  pinMode(MoteurFermeture, OUTPUT);
  pinMode(LedRouge, OUTPUT);
  pinMode(LedVerte, OUTPUT);

  digitalWrite(MoteurOuverture, LOW);
  digitalWrite(MoteurFermeture, LOW);

  // Initialisation liaison série matérielle pour remontée infos
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  delay(1000);
  Serial.println("Ethernet -> OK");
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.setBacklight(0x1);
  Serial.println("LCD -> OK");
  // Initialisation Module RFID via liaison série logicielle
  statusByte = myRFID.begin(&mySerial); //
  Serial.println("RFID -> OK");
  Serial.println("READY");
}

void loop() {
debut:

  if (digitalRead(11) == HIGH) {
    Serial.println("Relai ON");
  }

  if (nightMode == false) {
    lcd.clear();
    lcd.print("En attente carte");
    delay(1000);

    // Lecture status module RFID
    statusByte = myRFID.getUID();

    // Lecture carte RFID Ok ?
    if (statusByte == 0x86 )
    {
      // Affichage de l'UID sur moniteur série
      Serial.print("UID : ");

      oct1 = myRFID.readUID(1);
      oct2 = myRFID.readUID(2);
      oct3 = myRFID.readUID(3);
      oct4 = myRFID.readUID(4);
      oct5 = myRFID.readUID(5);
      oct6 = myRFID.readUID(6);
      oct7 = myRFID.readUID(7);
      oct8 = myRFID.readUID(8);

      Serial.print(oct1);
      Serial.print(oct2);
      Serial.print(oct3);
      Serial.print(oct4);
      Serial.print(oct5);
      Serial.print(oct6);
      Serial.print(oct7);
      Serial.println(oct8);

      String pageValue = RfidConnectRead();

    } else {
      goto debut;
    }
  } else { // Si on est en mode nuit

    lcd.print("Systeme OFF");

    goto debut;
  }
  goto debut;
}

String RfidConnectRead() {

  if (client.connect(server, 80)) {
    client.flush();
    Serial.flush();
    Serial.println("Connecte au serveur !");
    // Requête HTTP !!
    client.print("GET /VigiVelo/Checkcard.php?code=");
    client.print(oct1);
    client.print(oct2);
    client.print(oct3);
    client.print(oct4);
    client.print(oct5);
    client.print(oct6);
    client.print(oct7);
    client.print(oct8);
    client.println(" HTTP/1.1");
    client.println("Host: 172.16.102.14");
    client.println();
    return readPage();
  }
}

// Fonction pour extraire le texte de la page !!

String readPage() {
  //On lit la page en enlevant les balises { et }

  stringPos = 0;
  memset( &inString, 0, 32 ); //On libere la mémoire car cette fonction en bouffe a mort --'

  while (true) {

    if (client.available()) {
      char c = client.read();

      if (c == '{' ) { // { est notre caractère qui indique le debut de la reponse
        startRead = true; // Prêt à lire
      } else if (startRead) {

        if (c != '}') { // } est notre caractère qui indique la fin de la reponse
          inString[stringPos] = c;
          stringPos ++;
        } else {
          // On a ce qu'on veut ! On se déconnecte
          startRead = false;
          client.stop();
          client.flush();
          Serial.println("Fin de la reponse");
          Serial.println(inString);
          lcd.clear();
          lcd.print(inString);

          if ((String)inString == "CARTE OK") { // Si la réponse est favorable

            //if ((isLocked()) & digitalRead(openDoor) == HIGH) {
              digitalWrite(MoteurOuverture, HIGH);

              while (!isUnlocked());

              digitalWrite(MoteurOuverture, LOW);


          delay(3000);
          while ( digitalRead(openDoor) == LOW );
          delay(1000);
          digitalWrite(MoteurFermeture, HIGH);

          while (!isLocked());

          digitalWrite(MoteurFermeture, LOW);

            //}
          }
          else { // Si la reponse est défavorable
            lcd.clear();
            lcd.print("CARTE NON");
            delay(3000);          
          }
          return inString;

        }


      }
    }
  }
}


int isLocked(void)
{
  int ret = 0;
  digitalWrite(cmdOptoLock, HIGH);
  delay(1);
  ret = digitalRead(readOpto);
  digitalWrite(cmdOptoLock, LOW);

  return 1 - ret;
}

int isUnlocked(void)
{
  int ret = 0;
  digitalWrite(cmdOptoUnlock, HIGH);
  delay(1);
  ret = digitalRead(readOpto);
  digitalWrite(cmdOptoUnlock, LOW);

  return 1 - ret;
}
