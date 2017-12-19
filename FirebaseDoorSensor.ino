#include <ESP8266WiFi.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

const int switchPin = 1;
int lastState = 0;

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  pinMode(switchPin, INPUT);

}

void loop() {
  int curState = digitalRead(switchPin) == LOW ? 0 : 1;
  
}
