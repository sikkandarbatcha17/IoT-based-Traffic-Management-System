#include"FirebaseESP8266.h"
#include<ESP8266WiFi.h>
FirebaseData firebaseData;

#define FIREBASE_HOST "esp8266firebase1999.firebaseio.com"
#define FIREBASE_AUTH "jPwjDsBZX17qiFSTKEwkkyZWPjEukU4l4uazdj5l"
#define WIFI_SSID "WiFi"
#define WIFI_PASSWORD "siyam1999"
#define SS_PIN D2  //D2
#define RST_PIN D1 //D1

#include <SPI.h>
#include <MFRC522.h> //library for RFID 
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;

String fireStatus="";
int led=D3;                             
String path1="E5 78 34 1C";

void setup() 
{
  Serial.begin(9600);
  
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(led,OUTPUT);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("connecting to");
  Serial.print(WIFI_SSID);
  while(WiFi.status()!=WL_CONNECTED)
  {                                                                                                              
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected to");
  Serial.println(WIFI_SSID);
  Serial.print("IP ADDRESS IS:");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

  Firebase.setString(firebaseData,path,"0"); // first argumwent is the tag name
  
  
}
int read_Firebase()
{
  Firebase.readStream(firebaseData);
  if(firebaseData.streamTimeout())
  {
    Serial.println("timeout");
  }
  if(firebaseData.streamAvailable())
  {
    if(firebaseData.dataType()=="string")
    {
      fireStatus=firebaseData.stringData();


      Serial.println(!fireStatus);
      if(fireStatus=="1")
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }
}

void loop() 
{
  int rfid=read_rfid();
  if(rfid==1)
  {
      Serial.print("Detected");
      digitalWrite(led,HIGH);
                Firebase.setString(firebaseData,path1,"1"); // first argumwent is the tag name


  }
  else
  {
  Firebase.setString(firebaseData,path1,"0"); 
   digitalWrite(led,LOW);

  }
  delay(2000);
}

int read_rfid()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return 0;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return 0;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "E5 78 34 1C") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    delay(1000);
    Serial.println();
    statuss = 1;
    return 1;
  }
  
  else   {
    Serial.println(" Access Denied ");
    delay(3000);
    return 0;
  }
}
