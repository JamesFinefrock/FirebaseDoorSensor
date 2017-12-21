#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define FB_PATH ""
#define ID_PATH ""
#define NAME_PATH ""
#define OPEN_PATH ""
#define LASTOPENED_PATH ""

#define DOOR_ID ""
#define DOOR_NAME ""

const int switchPin = 1;
int lastState = LOW;

void setup() {
  Serial.begin(115200);

  connectWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  initializeDB();

  pinMode(switchPin, INPUT);
}

void loop() {
  int curState = digitalRead(switchPin);
  
  if(curState != lastState) {
    boolean isOpen = curState == HIGH ? true : false;
    Firebase.setBool(FB_PATH DOOR_ID OPEN_PATH, isOpen);
    lastState = curState;
  }
  
  delay(500);
}

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void initializeDB() {
  Firebase.setInt(FB_PATH DOOR_ID ID_PATH, atoi(DOOR_ID));
  Firebase.setString(FB_PATH DOOR_ID NAME_PATH, DOOR_NAME);
  Firebase.setBool(FB_PATH DOOR_ID OPEN_PATH, false);
}

