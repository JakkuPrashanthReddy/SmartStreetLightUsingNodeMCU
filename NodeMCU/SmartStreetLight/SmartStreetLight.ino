#include "ThingSpeak.h"
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <ESP8266WiFi.h>

unsigned long last_time = 0;
unsigned long timer_delay = 15000;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;
// your network key index number (needed only for WEP)
int flag = 0;
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Measure Analog Input (A0)
  int valueA0 = analogRead(A0);
  Serial.println("SensorValue:"+String(valueA0));
  if(valueA0 < 30 && flag ==0)
  {
    Serial.println("Street lights turned ON");
    digitalWrite(D1,HIGH);
    digitalWrite(D2,HIGH);
    flag = 1;
  }
  else if(valueA0 > 30 && flag == 1)
  {
    Serial.println("Street lights turned OFF");
    digitalWrite(D1,LOW);
    digitalWrite(D2,LOW);
    flag = 0;
  }
  if ((millis()-last_time) > timer_delay) 
  {
  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, valueA0, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
   last_time = millis();
  }
  delay(50);
}
