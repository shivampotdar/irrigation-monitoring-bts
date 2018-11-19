/*Arduino (MEGA) Code for collecting data from soil moisture sensors and DHT sensor and sending it to database using ESP8266EX WiFi Module
 * Connections:
 *  ESP   | Arduino
   *    3V3 | 3.3V
   *    GND | GND
   *    EN  | 3.3V
   *    TX  | RX1 (19)
   *    RX  | TX1 (18)
 *AT commands are the serial commands which is working here as a module to send data collected by Arduino to website/app
 *Authors: Shivam Potdar, Vithik Shah, Ayush Kumar (NITK Surathkal)
 *BTS Hackathon 2018 Project: Smart Irrigation Monitoring System
 */
#include<DHT.h>                                               //header file for interfacing with DHT11 temperature-humidity sensor

int aval[6], h[6], i = 0;                                     //aval reads analog values and h is humidity values mapped to 100
String AP = "Shivam", pwd = "starwars123";                    //AP name (SSID) and password of the desired WiFi network
String sp, sendData;                                          //sp stores most recently read value from the Arduino's receiving buffer, sendData stores the data to be sent over TCP
int freq = 25;                                                //Frequency (in s) for updating the moisture data

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);                                      //object of DHT class

void(* resetFunc) (void) = 0;                                   

bool findesp(void)                                            //Checks if ESP module is connected properly to Arduino and taking commands. When working as expected, ESP returns OK if you send command AT
{
  Serial1.print("AT\r\n");
  delay(100);
  if (Serial1.available() > 0)
  {
    sp = Serial1.readString();
    if (sp.indexOf('O') > 0)                                  //String.indexOf('var') checks for presence of 'var' in the given String and returns its index position
    {
      Serial.println("ESP Connected! :)");
      return 1;
    }

    else
    { 
      Serial.println("Check ESP connection! :(");
      return 0;
    }
  }
  else
  {
    Serial.println("Can't detect ESP connection! :(");
    return 0;
  }
}

void readmap(void)                                            //This function reads values of sensors from all the analog pinsm, and maps them on the scale of 0-100
{
  aval[0] = analogRead(A0);
  aval[1] = analogRead(A1);
  aval[2] = analogRead(A2);
  aval[3] = analogRead(A3);
  aval[4] = analogRead(A4);
  aval[5] = analogRead(A5);
  for (i = 0; i < 6; i++)
    h[i] = map(aval[i], 0, 1023, 100, 0);
}

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(DHTPIN,INPUT);
  dht.begin();
  Serial.begin(115200);
  Serial1.begin(115200);
  if (findesp() == 1)
  {
    Serial.println("ESP Found!");
    Serial1.print("AT+CWQAP\r\n");                            //This command disconnects any existing WiFi connection, if applicable
    delay(100);
    Serial1.print("AT+CWJAP=");                               //AT+CWJAP connects to the WiFi network with AP and pwd as passwords
    Serial1.print(AP);
    Serial1.print("\",");
    Serial1.print(pwd);
    Serial1.print("\"\r\n");
    delay(10000);
    sp = Serial1.readString();
    if (sp.indexOf("IP") >= 0)                                //On successful connection, ESP returns WIFI CONNECTED/ WIFI GOT IP
      Serial.println("WiFi Connected!");
    else
      Serial.println("Cannot Connect to WiFi!");
  }
  else
  {
    Serial.println("ESP not found! Resetting...");
    delay(1000);
    resetFunc();                                              //Soft resets the Arduino
  }
}

void loop()
{
  float hum = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false); //read from DHT
  if (isnan(hum) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature in the farm= ");
  Serial.println(t);
  Serial.print("Humidity in atmosphere= ");
  Serial.println(hum);
  if (findesp() == 1)
  {
    readmap();
    Serial.println("Humidity values from sensors:");
    for (i = 0; i < 6; i++)
    {
      Serial.print("S");
      Serial.print(i + 1);
      Serial.print(" = ");
      Serial.println(h[i]);
    }
    Serial1.print("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");  //establish TCP connection with Thingspeak API
    delay(1000);
    sp = Serial1.readString();
    if (sp.indexOf("OK") > 0)
    {
      Serial.print("\nConnected to Thingspeak!\n");
      sendData = "GET https://api.thingspeak.com/update.json?api_key=MWWO67QTVZPGUCEH&field1="; //Send GET request to my channel
      sendData += h[0];
      sendData += "&field2=";
      sendData += h[1];
      sendData += "&field3=";
      sendData += h[2];
      sendData += "&field4=";
      sendData += h[3];
      sendData += "&field5=";
      sendData += h[4];
      sendData += "&field6=";
      sendData += h[5];
      sendData += "&field7=";
      sendData += t;
      sendData += "&field8="
      sendData += hum;
      sendData += "\n";
      Serial1.print("AT+CIPSEND=");
      Serial1.print(sendData.length());                       //number of characters to be sent
      Serial1.print("\r\n");
      sp = Serial1.readString();
      if (sp.indexOf("OK") >= 0)                              //ESP returns OK when ready to send data
      {
        Serial1.print(sendData);
        sp = Serial1.readString();
        if (sp.indexOf("SEND OK") >= 0)
        {
          Serial.println("Data Sent successfully! :)");       //ESP returns SEND OK data is sent successfully
          delay(freq * 1000);                                   
        }
        else
          Serial.println("SendData failed :(");
      }
      else
        Serial.println("Connection error");
    }
    else
      Serial.println("Couldn't establish TCP connection with ThingSpeak!");
  }
  else
    Serial.print("ESP not found!");
  delay(2000);
}
