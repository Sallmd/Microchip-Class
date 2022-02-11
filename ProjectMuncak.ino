//library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WifiClient.h>
#include <WiFiManager.h>
 WiFiClient client;
 HTTPClient http; 

//display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN D5    


#define DHTTYPE    DHT11    

DHT dht(DHTPIN, DHTTYPE);

  int  red = D6;
  int  yellow =D7;
  int  green = D8;
  int  cold = 20;
  int  hot = 32;
  int  buzzer = D0;
  int  infraread = A0;
  int  night = 150;
  int  value;

const char server[] = "http://sall.rsxms.com/Pkg/receiver.php";
 WiFiManager wm;

void setup() {
    Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
   WiFi.mode(WIFI_STA); 
    Serial.begin(115200);
    bool res;
    
    res = wm.autoConnect("Esp8266","password"); // Ubah SSID dan password sesuai yang kalian mau

    if(!res) {
        Serial.println("Failed to connect");
       
    } 
    else {
  
        Serial.println("connected...yeey :)");
    }


  //Untuk menghapus tampilan bawaan adafruit

  //masukkan code setup ente disni:
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(infraread, INPUT);
  pinMode(hot, OUTPUT);
  pinMode(cold, OUTPUT);
  pinMode(DHTPIN, INPUT);
  Serial.println("DHTxx test!");
  dht.begin();
}
  
String statuss(){
  if (value > night) {
     String stat  = "Siang";
     return stat;
    } else {
     String stat  = "Malam";
     return stat;
    }
  }
   
void loop() {
  value = analogRead(infraread);

  display.setTextSize(0);
  display.setTextColor(SSD1306_WHITE);

  //untuk membaca sensor kelembapan dan suhu
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  
  if (WiFi.status()== WL_CONNECTED) {
   
    http.begin(client, server);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "temperature="
    +                (String) t
    +   "&humidity="+(String) h
    +        "&day="+ statuss();
   

    Serial.print("httpRequestData : ");
    Serial.println(httpRequestData);

    int httpResponCode = http.POST(httpRequestData);

    if(httpResponCode) {
      Serial.print("HTTP Respon Code : ");
      Serial.println(httpResponCode);
    }
    else {
      Serial.print("Error Code: ");
      Serial.println(httpResponCode);
    }     
    http.end();  
 }

   else {
    Serial.println("WiFi Disconnected");
    }
    delay(3000);
  // clear display
  display.clearDisplay();
  Serial.print("Kelembapan : ");
  Serial.print(h);
  Serial.println("%");
  display.setCursor(0, 0);
  display.println("Assalamualaikum");
  display.print("Humidity : ");
  display.print(h);
  display.println(" %"); 




 
  //Suhu Normal
  if (t > cold && t < hot){
     Serial.print("Temperature : ");
     Serial.print(t);
     Serial.println(" Celcius");
     Serial.println("Suhu Aman Bossqu");
     display.print("Temperature : ");
     display.print(t);
     display.println(" C");
     display.println("Suhu Aman Boss!");
     digitalWrite(green, HIGH);
  }
  //Suhu Rendah
  if (t < cold){
     Serial.print("Temperature : ");
     Serial.print(t);
     Serial.println(" Celcius");
     Serial.println("Suhu Dingin Boss");
     display.print("Temperature : ");
     display.print(t);
     display.println(" C");
     display.println("Suhu Dingin Boss!");
     digitalWrite(yellow, HIGH);
     digitalWrite(buzzer, HIGH);
     delay(750);
     digitalWrite(buzzer, LOW);
     delay(750);
  } 
  //Suhu Tinggi
  if (t > hot){
     Serial.print("Temperature : ");
     Serial.print(t);
     Serial.println(" Celcius");
     Serial.println("Suhu Tinggi");
     display.print("Temperature : ");
     display.print(t);
     display.println("C");
     display.println("Suhu Panas Boss!");
     digitalWrite(red, HIGH);
     digitalWrite(buzzer, HIGH);
     delay(750);
     digitalWrite(buzzer, LOW);
     delay(750);
  } 
  
 //untuk menentukan siang dan malam dengan infra read
 if (value > night) {
  Serial.println("Siang Bolong");
  Serial.println(" ");
  display.println("Siang Bolong");
  display.println(" ");
 }
 else {
  Serial.println("Udeh Malem Cuyy!");
  Serial.println(" ");
  display.println("Udah Malem Cuyy!");
  display.println(" ");
 }
   display.display(); 
 delay(1000);
 display.clearDisplay();

}

 
