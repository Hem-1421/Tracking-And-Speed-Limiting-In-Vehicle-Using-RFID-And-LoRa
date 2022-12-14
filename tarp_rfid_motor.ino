#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;          
int blockNum = 2;  
byte bufferLen = 18;
byte readBlockData[18];
                              //Motor A
const int inputPin1  = 5;    // Pin 15 of L293D IC
const int inputPin2  = 4;    // Pin 10 of L293D IC
                              //Motor B
const int inputPin3  = 6;    // Pin  7 of L293D IC
const int inputPin4  = 8;    // Pin  2 of L293D IC
//int EN1 = 5;                 // Pin 1 of L293D IC
//int EN2 = 6;                 // Pin 9 of L293D IC
int speed;

String content="";

void setup()
{
Serial.begin(9600);   // Initiate a serial communication
SPI.begin();      // Initiate  SPI bus
mfrc522.PCD_Init();   // Initiate MFRC522
Serial.println();

for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  
//pinMode(EN1, OUTPUT);   // where the motor is connected to
  //  pinMode(EN2, OUTPUT);   // where the motor is connected to
    pinMode(inputPin1, OUTPUT);
    pinMode(inputPin2, OUTPUT);
    pinMode(inputPin3, OUTPUT);
    pinMode(inputPin4, OUTPUT); 
    analogWrite(inputPin1,255);
  digitalWrite(inputPin2,LOW);
  analogWrite(inputPin3,255);
  digitalWrite(inputPin4,LOW); 
  speed=180;
//analogWrite(EN1, speed);
//analogWrite(EN2, speed);
   
}
void loop(){

if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("\n");
  Serial.println("**Card Detected**");
  /* Print UID of the Card */
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print("\n");
  /* Print type of card (for example, MIFARE 1K) */
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
         
  
   /* Read data from the same block */
   Serial.print("\n");
   Serial.println("Reading from Data Block...");
   ReadDataFromBlock(blockNum, readBlockData);
   /* If you want to print the full memory dump, uncomment the next line */
   mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
   
   /* Print the data read from block */
   Serial.print("\n");
   Serial.print("Data in Block:");
   Serial.print(blockNum);
   Serial.print(" --> ");
   for (int j=0 ; j<16 ; j++)
   {
     //Serial.write(readBlockData[j]);
     content +=(char(readBlockData[j]));

   }
   Serial.print(content);
   Serial.print("\n");
   if(content.equals("Speed-Limit-50- "))
   {
    analogWrite(inputPin1,150);
  digitalWrite(inputPin2,LOW);
  analogWrite(inputPin3,150);
  digitalWrite(inputPin4,LOW);
  Serial.println("speed limit: 50 KMPH");  
   }
   if(content.equals("Speed-Limit-90- "))
   {
    analogWrite(inputPin1,200);
  digitalWrite(inputPin2,LOW);
  analogWrite(inputPin3,200);
  digitalWrite(inputPin4,LOW);   
  Serial.println("speed limit: 90 KMPH");  

   }
   content="";
}





void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
  
}


 /*
// Look for new cards
if ( ! mfrc522.PICC_IsNewCardPresent())
{
return;
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial())
{
return;
}
//Show UID on serial monitor
Serial.print("UID tag :");
String content= "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++)
{
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
Serial.print(mfrc522.uid.uidByte[i], HEX);
content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
Serial.println();
content.toUpperCase();
if (content.substring(1) == "50 31 0D 19") //change here the UID of the card/cards that you want to give access
{
Serial.println();
Serial.println("speed 50 kmph");
speed=151;
        
        analogWrite(EN1, speed); // from rfid (speed*100/255)
        analogWrite(EN2, speed);
        Serial.println(speed*100/255);
delay(1000);
}
else if (content.substring(1) == "E9 F4 16 63") //change here the UID of the card/cards that you want to give access
{
Serial.println();
Serial.println("speed 100 kmph");
speed=255;
 analogWrite(EN1, speed);
 analogWrite(EN2, speed);
 Serial.println(speed*100/255);
delay(1000);
}
else   {
speed=180;
analogWrite(EN1, speed);
analogWrite(EN2, speed);
Serial.println(speed*100/255);
}
}*/
