#include <ESP8266WiFi.h> 
#include <dht11.h>
#include <FirebaseArduino.h> 

#define DHT11PIN 5

dht11 DHT11;

#define FIREBASE_HOST "pametna-kuca.firebaseio.com"  //naša baza podatka 
#define FIREBASE_AUTH "n3c24gpqkzqEHebHTx24LNGeJi7cNn50sIbrv0ov" //kljuc za autentifikaciju baze 

//vatra
int flame_sensor = 14;
int flame_detected;
int buzzer = 13;

const int flamePin = 14;
int Flame = HIGH;


//prozor i ventilacija
int ventilacija = 12;
int prozor = 15;

//gasni
int greenLed = 13;
int smokeA0 = A0;
int sensorThres = 400;



//int outputpin= A0;
#define WIFI_SSID "Balkanika" //ime wifi mreže
#define WIFI_PASSWORD "12344321" //sifra

//kisa
const int capteur_D = 4;

void setup()
{
  Serial.begin(9600);

    WiFi.disconnect();               
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);  
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    Serial.print(" ");
    while((!(WiFi.status() == WL_CONNECTED))){
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Address is : ");
    Serial.println(WiFi.localIP()); 
    
    Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); 

    
    //gas
     pinMode(greenLed, OUTPUT);
     pinMode(smokeA0, INPUT);

    //vatra
    //pinMode(flame_sensor, INPUT);
    pinMode(flamePin, INPUT);
    
    //kisa
    pinMode(capteur_D, INPUT);

       //prozor i ventilacija
    pinMode(ventilacija,OUTPUT);
    pinMode(prozor,OUTPUT);

    //biper
    pinMode(buzzer, OUTPUT);
}

String kisa;
String pozar;
String gas;

void loop()
{

    //sensor kise
    if(digitalRead(capteur_D) == LOW) 
    {
      Serial.println("Pada kiša"); 
      kisa = "Pada kiša";
      delay(10); 
     }
    else
    {
      Serial.println("Ne pada kiša");
      kisa = "Ne pada kiša";
      delay(10); 
     }
     Firebase.setString("kisa", kisa);  
      if (Firebase.failed()) {
        Serial.print("setting /kisa failed:");
        Serial.println(Firebase.error());  
        return;
    }
  //sensor temperature
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Vlažnost vazduha (%): ");
  float h = ((float)DHT11.humidity);
  Serial.println(h);
  //Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperatura (C): ");
  float t = ((float)DHT11.temperature);
  Serial.println(t);
  //Serial.println((float)DHT11.temperature, 2);

    Firebase.setFloat("temp", t);  
      if (Firebase.failed()) {
        Serial.print("setting /temp failed:");
        Serial.println(Firebase.error());  
        return;
    }
    Firebase.setFloat("vlaznost", h);  
      if (Firebase.failed()) {
        Serial.print("setting /vlaznost failed:");
        Serial.println(Firebase.error());  
        return;
    }
//vatra

  Flame = digitalRead(flamePin);
  if (Flame == LOW)
  {
    Serial.println("Požar");
    pozar = "Požar";
    tone(buzzer, 1000, 200);
    
  }
  else
  {
    Serial.println("No worries");
    pozar = "Nema požara";
    noTone(buzzer);
  }
  Firebase.setString("pozar", pozar);  
      if (Firebase.failed()) {
        Serial.print("setting /temp failed:");
        Serial.println(Firebase.error());  
        return;
    }
//sensor gasa
int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);

if (analogSensor > sensorThres)
  {
    digitalWrite(greenLed, HIGH);
    gas = "Zagušljivo";
  }
  else
  {
    digitalWrite(greenLed, LOW);
    gas = "Nije zagušljivo ";
  }
  Firebase.setString("co2", gas);  
      if (Firebase.failed()) {
        Serial.print("setting /temp failed:");
        Serial.println(Firebase.error());  
        return;
    }


    //prozor i ventilacija

    if(h > 60 || t > 30 || analogSensor > sensorThres)
    {
      if(digitalRead(capteur_D) == LOW)
      {
        digitalWrite(prozor, LOW);
        digitalWrite(ventilacija, HIGH);
      }
      else
      {
        digitalWrite(prozor, HIGH);
        digitalWrite(ventilacija, LOW);
      }
    }
//delay(500);

}
