#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define FB_PATH ""

const int switchPin = 1;
int lastState = LOW;

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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(switchPin, INPUT);
}

void loop() {
  int curState = digitalRead(switchPin);
  if(curState != lastState) {
    boolean isOpen = curState == HIGH ? true : false;
    Firebase.setBool(FB_PATH, isOpen);
    lastState = curState;
  }
  
  delay(500);
}
