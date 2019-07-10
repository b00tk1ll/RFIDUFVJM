#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseArduino.h>
#include <DNSServer.h>
#include <WiFiManager.h> 

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define FIREBASE_HOST ".firebaseio.com"
#define FIREBASE_AUTH ""

String content= "";
String carderror = "";
String card;
String cardtest ;
String nome;
String refnome;
int permissao;
String refpermissao;
byte letter;
int statuss = 0;
int out = 0;
int card1;
int presence;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Passa parametros para bilbioteca RFID.
void setup(){
    Serial.begin(9600);   //Inicia Comunicação Serial
  
    WiFiManager wifiManager;
    //wifiManager.resetSettings();
    wifiManager.autoConnect("RFIDUFVJM","12345678");
    Serial.println("conectado :)");
      
    SPI.begin();      // Inicia SPI
    mfrc522.PCD_Init();   // Inicia MFRC522
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop(){
 
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())  {
    return;
  }
  
  Serial.println();
  Serial.print("Card RFID nº:");
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     card.concat(String(mfrc522.uid.uidByte[i], HEX));  
  }
  String refnome = String(card+"/nome");
  String refpermissao = String(card+"/permissao");
  nome = Firebase.getString(refnome);
  permissao = Firebase.getInt(refpermissao);
  card1 = Firebase.getInt("card");
  presence = Firebase.getInt("presence");
  Serial.println(cardtest);
    if (nome == carderror){
      Serial.println("Cartão não cadastrado");
    }
    else{
    if(permissao == 1 && card1 == 1 && presence == 0){
        Serial.print("Olá ");
        Serial.print(nome);
        Serial.println(" seja bem-vindo");
        Firebase.setInt("card", 0);
        Firebase.setInt("presence", 1);
        Firebase.setInt("btn", 1);
      }
    else if (permissao == 1 && presence == 1 && card1 == 1){
        Serial.print("Olá ");
        Serial.print(nome);
        Serial.println(" até mais");
        Firebase.setInt("card", 0);
        Firebase.setInt("presence", 0);
        Firebase.setInt("btn", 0);
      }
    else{
        Serial.print("Olá ");
        Serial.print(nome);
        Serial.println(" Infelizmente tu não tem permissão");
      }
    }
 card="";
}
