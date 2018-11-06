/* ACM IoT SMP 2018 Project: SHIVAM POTDAR, 2nd Year BTech EEE
 *  This project uses: Arduino UNO, ESP12-E chip, PIR sensor module, DHT 11 sensor module, ThingSpeak API
 *  Read data from DHT and PIR through Arduino, and send it to Thingspeak using ESP 12E module communicating via SoftwareSerial library with it.
 *  The buzzer and LED activate when PIR module detects motion.
 *  shivampotdar99@gmail.com/ +91-9511893050
 */

#include <DHT.h>
#include <SoftwareSerial.h>
#define DHTPIN 7
#define PIRin 4
#define LED 5
#define buzz 6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); //Obj of DHT class
SoftwareSerial esp(2,3); //Software serial with ESP12-E

String AP="Shivam Lappy",pwd="starwars123",sp,sendData,pirres="No";  
int flag=0;

void setup() 
{
  esp.begin(9600);
  Serial.begin(9600);
  dht.begin();
  pinMode(DHTPIN,INPUT);
  pinMode(PIRin,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(buzz,OUTPUT);
  //Initialise both LED and buzzer to 0
  digitalWrite(LED,LOW);
  digitalWrite(buzz,LOW);
  delay(1000);
  //DISCONNECT EXISTING CONNECTION IF ANY
  esp.print("AT+CWQAP\r\n");
  delay(2000);
  esp.print("AT+CWMODE_CUR=1\r\n"); //STA mode
  delay(1000);
  //CONNECT TO MY AP
  esp.print("AT+CWJAP=");
  esp.print("\"");  
  esp.print(AP);
  esp.print("\",");
  esp.print("\"");
  esp.print(pwd);
  esp.print("\"\r\n");
  while(millis()<20000) //Give 20secs for ESP to connect
  {
  sp=esp.readString();
  Serial.print("...");
  if(sp.indexOf("IP")>=0)
     {Serial.print("NOW CONNECTED\n");
     flag=1;
     break;}
  }
  if(flag==0)
  Serial.print("ERROR");
  delay(1000);
  //GIVE IP OF CURRENT STA MODE
  esp.print("AT+CIFSR\r\n");
  sp=esp.readString();
  Serial.println(sp);
  
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false); //read from DHT
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C \n");
  if(digitalRead(PIRin)==HIGH) //This block reads from PIR sensor
  {
     Serial.print("Motion Detected!");
     pirres="Yes";
     digitalWrite(LED,HIGH);
     delay(1000);
     digitalWrite(LED,LOW); 
     digitalWrite(buzz,HIGH);
     delay(500);
     digitalWrite(buzz,LOW);
   }
   else
   {
      Serial.print("No Motion!");
      pirres="No";
   }
  esp.print("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");  //establish TCP connection with Thingspeak API 
  delay(1000);
  sp=esp.readString();
  if(sp.indexOf("OK")>0)
    Serial.print("\nConnected to Thingspeak!\n");
  else
    Serial.print("Connection Failed!\n");
  sendData="GET https://api.thingspeak.com/update.json?api_key=N7YK0OWDETQK0VZ4&field1=";  //Send GET request to my channel
  sendData+=t;
  sendData+="&field2=";
  sendData+=h;
  sendData+="&field3=";
  sendData+=hic;
  sendData+="&field4=";
  sendData+=pirres;
  sendData+="\n";
  esp.print("AT+CIPSEND=");
  esp.print(sendData.length());
  esp.print("\r\n");
  delay(500);
  esp.print(sendData);
  sp=esp.readString();
  if(sp.indexOf("SEND OK")>0)
    Serial.print("Data Sent :)\n");
  else
    Serial.print("Failed :(\n");
  delay(16000); //thingspeak requires 15 secs gap between two updates
}
