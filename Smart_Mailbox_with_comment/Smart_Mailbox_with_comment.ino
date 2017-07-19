/*
 * Project Name: Smart Mailbox
 * 
 * Project Team leader: Jeongwhan Choi
 * Project Team name: 1532
 * 
 */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
 
const char* ssid = "*******";                          // 와이파이 이름(Wifi name)
const char* password = "**********";                     // 와이파이 비밀번호(Wifi pwd)
const char* accessToken = "o.TSh4FVHVkqFcuD5C0pOxPgC9GMPK6xKd";  // PushBullet token(토큰)
String Title = "<1532조 스마트 우편함>"; 
 
WiFiClientSecure client;

#define echo D7 // 에코 핀(echo pin)
#define trig D6 // 트리거 핀(trigger pin)
int def = 10; //기본 값 (default distance)
int temp; //측정 값을 임시로 저장할 변수 (temporary distance)

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
  delay(6000); //딜레이 6.0s
}
int distance(){
  digitalWrite(trig,HIGH); //trig pin에 HIGH 신호를 보낸다.
  delayMicroseconds(30); //10 microseconds (1/100,000s) 동안
  digitalWrite(trig, LOW); //trig pin에 LOW 신호를 보낸다.
  return pulseIn(echo,HIGH) *17 /1000; //echo pin이 다시 High 신호를 받기까지의 사간 340/2/10000 값을 반환
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


