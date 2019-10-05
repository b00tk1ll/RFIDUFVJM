// Including libraries
#include <SPI.h> // Communication library SPI
#include <MFRC522.h> // RFID library
#include <ESP8266WiFi.h> // WiFi management library for ESP8266
#include <ESP8266WebServer.h> // WebServer management library for ESP8266 
#include <FirebaseArduino.h> // Firebase library
#include <DNSServer.h> // DNS library
#include <WiFiManager.h> // WiFi configuration library
 
// Setting RFID pins and firebase configurations
#define SS_PIN 4  // D2
#define RST_PIN 5 // D1
#define FIREBASE_HOST ".firebaseio.com"
#define FIREBASE_AUTH ""

// Declaring variables
String content= "";
String carderror = "";
String card;
String name;
String refname;
int permition;
String refpermition;
byte letter;
int statuss = 0;
int out = 0;
int card1;
int presence;
int IN1 = 0; // D3
int IN2 = 2; // D4

MFRC522 mfrc522(SS_PIN, RST_PIN);   // send the parameters to the RFID library

void setup(){
  
    Serial.begin(9600);   // Start Serial Communication
  
    WiFiManager wifiManager; // Starts WIFI configuration
    //wifiManager.resetSettings();
    wifiManager.autoConnect("RFIDUFVJM","12345678"); // Cria o wifi para configuração
    Serial.println("conectado :)"); // Printa no Serial monitor se conectou
    
    //Define H bridge as output
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    
    SPI.begin(); // Start SPI
    mfrc522.PCD_Init(); // Start MFRC522
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Inicia Firebase
}

void loop(){
  // Check if you have passed the card
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Reads the card
  if (!mfrc522.PICC_ReadCardSerial())  {
    return;
  }
  // Line break and text
  Serial.println();
  Serial.print("Card RFID nº:");

  // concatenates the read data, saves in an variable and displays in serial monitor
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     card.concat(String(mfrc522.uid.uidByte[i], HEX));  
  }

  // Concatenates card number with variable name for firebase query
  String refname = String(card+"/name");
  String refpermition = String(card+"/permition");

  // Receive Firebase data and save to a local variable
  name = Firebase.getString(refname);
  permition = Firebase.getInt(refpermition);
  card1 = Firebase.getInt("card");
  presence = Firebase.getInt("presence");
    
   // If not registered returns error
    if (name == carderror){
      Serial.println("Cartão não cadastrado");
      // Turn on red LED to show that you have been denied access
      digitalWrite(16, HIGH);
      delay(3000);
      
      // Turn off red Led
      digitalWrite(16, LOW);
    }
    else{
    // Checks if the person is out and if are registered and release the input
    if(permition == 1 && card1 == 1 && presence == 0){
        // Displays in serial monitor "Hello someone, welcome!
        Serial.print("Olá ");
        Serial.print(name);
        Serial.println(" seja bem-vindo");

        // Turn on green LED to show that access was allowed
        digitalWrite(D8, HIGH);
        
        // Change Firebase variables
        Firebase.setInt("card", 0);
        Firebase.setInt("presence", 1);
        Firebase.setInt("btn", 1);
        
        // Spin motor and opens the door
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        delay(1600);

        // Lock motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        delay(6000);

        // Turn off green LED 
        digitalWrite(D8, LOW);
        
        // Spin motor and closes the door
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        delay(1500);
        
        // Lock motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
                
      }
    // Checks if the person is inside and if are registered and release the output
    else if (permition == 1 && presence == 1 && card1 == 1){
      
        // Displays in serial monitor "Hello someone, see you later"
        Serial.print("Hello ");
        Serial.print(name);
        Serial.println(" see you later");

        // Turn on green LED to show that access was allowed
        digitalWrite(D8, HIGH);

        // Change variables in Firebase
        Firebase.setInt("card", 0);
        Firebase.setInt("presence", 0);
        Firebase.setInt("btn", 0);
        
        // Spin motor and opens the door
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        delay(1600);

        // Lock motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        delay(6000);
        
        // Turn off green LED 
        digitalWrite(D8, LOW);
        
        // Spin motor and closes the door
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        delay(1500);
        
        // Lock motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
      }
    // Error if the user was registred but don't have acess permition 
    else{
        // Display in serial monitor "Hello someone you don't have acess permition"
        Serial.print("Hello ");
        Serial.print(name);
        Serial.println(" you don't have acess permition");

        // Turn on red LED to show that you have been denied access
        digitalWrite(D0, HIGH);
        delay(3000);

        // Turn off red LED
        digitalWrite(D0, LOW);
      }
    }
 // Reset card variable
 card="";
}
