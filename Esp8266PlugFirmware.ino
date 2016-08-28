#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
//int val;
const char* ssid     = "ABCDE";
//const char* ssid     = "ASELAB";
//const char* ssid     = "HateTigon";
//const char* password = "khongnho321";
//const char* password = "ASE11235813";
const char* password = "khongcopass";
#define SERVER   "184.106.153.149" // api.thingspeak.com
#define PORT     80                     // 80 = HTTP default port
#define SERVERIP "192.168.1.91"
// replace with your channel's thingspeak API key
#define apiReadKey "Y76PNEVSW3M2NNLL"
#define apiWriteKey "2QVOB3V2KNRR0EWQ"
#define channel "80425"
const char* host = "www.aselab.edu.vn";

void connectLocalWiFi() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}
void setup() {
  connectLocalWiFi();
  pinMode(D1, INPUT_PULLUP);           // set pin to input
  pinMode(D2, OUTPUT);
}

void loop() {
  int value = digitalRead(D1);
  Serial.print(F("connecting to "));
  Serial.println(F(SERVER));
  value = digitalRead(D2);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(SERVER, httpPort)) {
    Serial.println("connection failed");
    connectLocalWiFi();
    return;
  }
//  // We now create a URI for the request
  String url = "https://api.thingspeak.com/update?api_key=2QVOB3V2KNRR0EWQ&field1=";
  url += value;
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(200);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  delay(3000);
  // We now create a URI for the request
  url = "https://api.thingspeak.com/channels/80425/fields/1/last";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  
  delay(2000);
  // Read all the lines of the reply from server and print them to Serial
  while (client.available() > 0) {
    
    String val = client.readStringUntil('\r');
//    int val = client.read();
//    val = inByte.toInt();               
  }
  Serial.println(val);
  if (val == 1){      
      digitalWrite(D2, HIGH);
  }    
  else if (val == 0){
    digitalWrite(D2, LOW);
  }  
  delay(1000);
  
  Serial.println("closing connection");
}
