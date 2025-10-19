#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// const char** ssids = ["6866", "iPhone 1000"];
// const char** passwds = ["22521281", "qwertyuiop"];
const char* ssid = "iPhone 1000";
const char* passwd = "qwertyuiop";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client -> setInsecure();
    clearInput();

    HTTPClient https;
    String url = "";
    boolean resume = false;
    Serial.println("1 -> Get | 2 -> Write: ");
    char in;
    while(resume == false){
      if(Serial.available() > 0){
        if(Serial.available() > 0){
          in = Serial.read();
          resume = true;
        }
      }
    }
    resume = false;
    while(resume == false){
      if(in == '1'){
        url = "https://markymark.pythonanywhere.com/get/weight";
        resume = true;
      }
      else if(in == '2'){
        Serial.println("Please enter the weight: ");
        while(resume == false){
          clearInput();
          int weight = 0;
          if(Serial.available() > 0){
            delay(100);
            int weight = Serial.parseInt();
            url = "https://markymark.pythonanywhere.com/weight/" + String(weight);
            resume = true;
          }
        }
      }
    }
    resume = false;

    Serial.println(url);
    https.begin(*client, url);
    https.addHeader("Content-Type", "text/plain");

    int httpCode = https.POST("@2233");

    if (httpCode > 0){
      String payload = https.getString();
      Serial.println();
      Serial.println(payload);
    }
    https.end();
  }
  Serial.print("Wait for 3 seconds to enter next round...");
  delay(1000);
  Serial.print(".");
  delay(1000);
  Serial.println(".");
  delay(1000);
}
void clearInput(){
  while(Serial.read() >= 0);
}
