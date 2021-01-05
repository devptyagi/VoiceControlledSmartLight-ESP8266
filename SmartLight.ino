#include<ESP8266WiFi.h>

const char* ssid = "WIFI SSID";
const char* password = "PASSWORD";

WiFiServer server(80);
WiFiClient client;
int lightPin = 5;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, HIGH);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi Connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  client = server.available();
  if(!client) {
    return;
  }

  String request = checkClient();
  Serial.println(request);
  if(request.indexOf("ON") != -1) {
    digitalWrite(lightPin, LOW);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
  } else if(request.indexOf("OFF") != -1) {
    digitalWrite(lightPin, HIGH);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
  }
}

String checkClient (void) {
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
