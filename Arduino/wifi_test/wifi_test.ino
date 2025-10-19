#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

WiFiServer server(80);

const char* ssid = "iPhone 1000";
const char* password = "qwertyuiop";

void setup(){
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.begin(9600);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop(){
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.1.88:8090/helloesp"); //Specify request destination
 
    int httpCode = http.GET(); //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
 
    }else Serial.println("An error ocurred");
 
    http.end();   //Close connection
 
  }
 
  delay(10000);    //Send a request every 10 seconds
}