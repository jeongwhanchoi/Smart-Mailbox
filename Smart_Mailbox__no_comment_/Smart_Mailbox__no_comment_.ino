#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
 
const char* ssid = "*******";                        
const char* password = "**********";                 
const char* accessToken = "o.TSh4FVHVkqFcuD5C0pOxPgC9GMPK6xKd";  
String Title = "<1532조 스마트 우편함>"; 
 
WiFiClientSecure client;

#define echo D7 
#define trig D6 
int def = 10; 
int temp; 

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
  }
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
}
 
void loop() {
  temp = distance();
  Serial.println(temp);
  if(def>temp){
    sendToPushBullet("우편 도착");
    def = temp;
  }
  else if(def<temp){
    sendToPushBullet("우편 회수");
    def = temp;
  }
  else{
    def = temp;  
  }
  delay(6000);
}
int distance(){
  digitalWrite(trig,HIGH);
  delayMicroseconds(30);
  digitalWrite(trig, LOW);
  return pulseIn(echo,HIGH) *17 /1000;
} 
void sendToPushBullet(String Msg) {
  Msg = "{\"type\": \"note\", \"title\": \"" + Title + "\", \"body\": \"" + Msg + "\"}\r\n";
  if (client.connect("api.pushbullet.com", 443)) {
    client.println("POST /v2/pushes HTTP/1.1");
    client.println("Host: api.pushbullet.com");
    client.println("Authorization: Bearer " + String(accessToken));
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(Msg.length()) + "\r\n");
    client.print(Msg);  
    client.stop();
  }
}
