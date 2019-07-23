// Declarando das bibliotecas
#include <SPI.h> // Biblioteca de comunicação SPI
#include <MFRC522.h> // Biblioteca RFID
#include <ESP8266WiFi.h> // Bibblioteca de gerenciamento WiFi ESP8266
#include <ESP8266WebServer.h> // Biblioteca gerenciamaneto WebServer ESP8266 
#include <FirebaseArduino.h> // Biblioteca Firebase
#include <DNSServer.h> // Biblioteca DNS
#include <WiFiManager.h> // Biblioteca configuração WiFi
 
// Definindo pinos do rfid e configurações do firebase
#define SS_PIN 4  // D2
#define RST_PIN 5 // D1
#define FIREBASE_HOST "rfidufvjm.firebaseio.com"
#define FIREBASE_AUTH "0GFGoA9Kn51ii2LtOa6MDBU9tVt2gBWS5X4cGQBZ"

// Declarando as variaveis
String content= "";
String carderror = "";
String card;
String nome;
String refnome;
int permissao;
String refpermissao;
byte letter;
int statuss = 0;
int out = 0;
int card1;
int presence;
int IN1 = 0; // D3
int IN2 = 2; // D4

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Passa parametros para bilbioteca RFID.

void setup(){
  
    Serial.begin(9600);   // Inicia Comunicação Serial
  
    WiFiManager wifiManager; // Inicia configuração do WiFi
    //wifiManager.resetSettings();
    wifiManager.autoConnect("RFIDUFVJM","12345678"); // Cria o wifi para configuração
    Serial.println("conectado :)"); // Printa no Serial monitor se conectou
    
    //Declara pinos da Ponte H como saida
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    
    SPI.begin(); // Inicia SPI
    mfrc522.PCD_Init(); // Inicia MFRC522
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Inicia Firebase
}

void loop(){
  // Verifica se passou o cartão
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Lê o cartão
  if (!mfrc522.PICC_ReadCardSerial())  {
    return;
  }
  // Printa uma quebra sde linha e um texto
  Serial.println();
  Serial.print("Card RFID nº:");

  // Contatena os dados lidos, salva em uma variavel e printa no serial monitor
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     card.concat(String(mfrc522.uid.uidByte[i], HEX));  
  }

  // Concatena nº do cartão + a o nome da variavel para consulta no firebase
  String refnome = String(card+"/nome");
  String refpermissao = String(card+"/permissao");

  // Recebe os dados do Firebase e salva em uma variavel local
  nome = Firebase.getString(refnome);
  permissao = Firebase.getInt(refpermissao);
  card1 = Firebase.getInt("card");
  presence = Firebase.getInt("presence");
    
   // Se não estiver cadastrado retorna erro
    if (nome == carderror){
      Serial.println("Cartão não cadastrado");
      // Liga Led vermelho para mostrar que teve acesso negado
      digitalWrite(16, HIGH);
      delay(3000);
      
      // Desliga Led vermelho
      digitalWrite(16, LOW);
    }
    else{
    // Verifica se a pessoa está fora, se ta cadastrado e libera entrada
    if(permissao == 1 && card1 == 1 && presence == 0){
        // Printa no serial monitor Olá Fulano seja bem-vindo
        Serial.print("Olá ");
        Serial.print(nome);
        Serial.println(" seja bem-vindo");

        // Liga Led verde para mostrae que teve acesso consedido
        digitalWrite(D8, HIGH);
        
        // Muda variaveis no Firebase
        Firebase.setInt("card", 0);
        Firebase.setInt("presence", 1);
        Firebase.setInt("btn", 1);
        
        // Gira o Motor e abre a porta
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        delay(1600);

        // Trava Motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        delay(6000);

        // Desliga Led verde
        digitalWrite(D8, LOW);
        
        // Gira o Motor e fecha a porta
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        delay(1500);
        
        // Trava Motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
                
      }
    // Verifica se a pessoa esta dentro, se esta cadastrado e libera saida
    else if (permissao == 1 && presence == 1 && card1 == 1){
      
        // Printa no serial monitor Olá Fulano até mais
        Serial.print("Olá ");
        Serial.print(nome);
        Serial.println(" até mais");

        // Liga Led verde para mostrar que teve acesso consedido
        digitalWrite(D8, HIGH);

        // Muda variaveis no Firebase
        Firebase.setInt("card", 0);
        Firebase.setInt("presence", 0);
        Firebase.setInt("btn", 0);
        
        // Gira o Motor e abre a porta
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        delay(1600);

        // Trava Motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        delay(6000);
        
        // Desliga Led verde
        digitalWrite(D8, LOW);
        
        // Gira o Motor e fecha a porta
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        delay(1500);
        
        // Trava Motor
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
      }
    // Erro se a pessoa esta cadastrada mas não tem permissão de entrada
    else{
        // Printa no serial monitor Olá Fulano tu não tem permissão
        Serial.print("Olá ");
        Serial.print(nome);
        Serial.println(" Infelizmente tu não tem permissão");

        // Liga Led vermelho para mostrar que teve acesso negado
        digitalWrite(D0, HIGH);
        delay(3000);

        //Desliga Led vermelho
        digitalWrite(D0, LOW);
      }
    }
 // Zera variavel card
 card="";
}
