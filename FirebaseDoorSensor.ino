#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define TIME_API_KEY ""

#define FCM_SERVER_KEY ""
#define FCM_TOPIC ""

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

  getTime();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  initializeDB();

  pinMode(switchPin, INPUT);
}

void loop() {
  int curState = digitalRead(switchPin);
  
  if(curState != lastState) {
    boolean isOpen = curState == HIGH ? true : false;
    Firebase.setBool(FB_PATH DOOR_ID OPEN_PATH, isOpen);

    if(isOpen) {
      sendFCM();
    }
    
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
  Firebase.setString(FB_PATH DOOR_ID LASTOPENED_PATH, "");
}

void sendFCM() {
  WiFiClientSecure client;
  
  String data = "{";
  data = data + "\"to\": \"/topics/" FCM_TOPIC "\",";
  data = data + "\"data\": {";
  data = data + "\"name\" : \"" DOOR_NAME "\" ";
  data = data + "} }";
  
  Serial.println("Send data...");
  
  if (client.connect("fcm.googleapis.com", 443)) {
    Serial.println("Connected to the server..");
    client.println("POST /fcm/send HTTP/1.1");
    client.println("Host: fcm.googleapis.com");
    client.println("Authorization: key=" FCM_SERVER_KEY);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
  }
  
  Serial.println("Data sent...Reading response..");
  
  while (client.available()) {
   char c = client.read();
   Serial.print(c);
  }
  
  Serial.println("Finished!");
  client.flush();
  client.stop();
}

String getTime() {
  WiFiClient client;
  char* host = "api.timezonedb.com";
  String url = "/v2/get-time-zone?key=" TIME_API_KEY "&format=json&by=zone&zone=America/New_York";

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
}

