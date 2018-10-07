//
// For Wemos D1 Mini (or similar ESP-based board)
// to monitor if a door was left open - generally used for garage door monitoring
//
// Reed switch triggers LED light on after slight delay, tracks open time, and sends updates to the server
//
// Using internal pullup, so no external resistor needed (it would go to 3.3v otherwise)
// D4 - switch - GND  // likely a reed switch.
//
// Server Expects updates in the form:
// http://yourserver.com/yourdoorstatusfolder/updateDoorStatus.php?secret=somethingsecret&doorID=1&status=CLOSED&opentime=3:44
//
// Where:
//  secret must match the password in the php file
//  doorID is the ID of the pump being tracked (the server can handle multiple pumps)
//  status is either OPEN or CLOSED
//  opentime is usually only included for status CLOSED, and it indicates how long the door was OPEN.
//  opentime should be left out for status OPEN, or set to blank, unless it's an alert situation - the server will send an alert email if OPEN includes a opentime value.
//

#include <ESP8266WiFi.h>

const char* ssid     = "yourSSID";  // replace with your own WiFi network name
const char* password = "yourWiFiPassword"; // replace with your own WiFi network password
 
const char* host = "www.yourserver.com";
const char* url = "/yourdoorstatusfolder/updateDoorStatus.php?secret=somethingsecret";  // update the secret to match your server side password in the php script

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;

// identify this specific sensor with a unique ID (for the server to tell them apart)
String doorID = "1";

byte SWITCH = D4;
byte ledPin = BUILTIN_LED; // built in LED is D4
bool door_open = false;
unsigned long starttime;
unsigned long opentime;

#define LED_ON  LOW
#define LED_OFF HIGH

#define DOOR_OPEN   HIGH
#define DOOR_CLOSED LOW

#define SECOND 1000ul
#define MINUTE (60ul * SECOND)

#define WARNING_THRESHOLD (MINUTE * 5ul)  // 5 minutes
#define WARNING_REPEAT_THRESHOLD MINUTE   // repeat the warning once per minute
unsigned long nextwarning;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  digitalWrite(ledPin, LED_OFF);   // turn the ledPin off

  nextwarning = WARNING_THRESHOLD;

  Serial.begin(115200);
  Serial.println("\n\nStarting...\n");

  delay(100);

  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());

}

void loop() {
  bool new_door_open = digitalRead(SWITCH) == DOOR_OPEN;  // read the value from the switch

//  Serial.print("new_door_open = ");
//  Serial.println(new_door_open);

  // check if pump status has changed
  if (new_door_open != door_open) {
    door_open = new_door_open;

    if (door_open) {
      starttime = millis();
      Serial.println("Door is OPEN");
      digitalWrite(ledPin, LED_ON);
      updateServer(true, "");
    }
    else {
      opentime = millis() - starttime;
      String timeString =  timeStringFromMillis(opentime);
      Serial.println("Door is CLOSED. It was open for " + timeString);
      digitalWrite(ledPin, LED_OFF);
      nextwarning = WARNING_THRESHOLD; // reset the threshold
      updateServer(false, timeString);
    }

  }

  if (door_open && millis() - starttime > nextwarning) {
    opentime = millis() - starttime;
    String timeString =  timeStringFromMillis(opentime);
    Serial.println("WARNING: Door is still OPEN. It has been open for " + timeString);
    nextwarning += WARNING_REPEAT_THRESHOLD;
    updateServer(true, timeString);
  }

  delay(1000);  // wait one second
}

String timeStringFromMillis(unsigned long milliseconds) {
  unsigned long allseconds = (milliseconds + 500ul) / 1000;
  int minutes = allseconds / 60;
  int seconds = allseconds % 60;

  String timeString = "";
  
  if (minutes < 10) {
    timeString = timeString + "0";
  }
  timeString = timeString + String(minutes) + ":";

  if (seconds < 10) {
    timeString = timeString + "0";
  }
  timeString = timeString + String(seconds);

  return timeString;
}

void updateServer(bool door_is_open, String openTime) {
  String strStatus = door_is_open ? "OPEN" : "CLOSED";
  
  // connect to the host
  Serial.println("Making the connection to the host...");
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Sending request with new status: door is ");
  Serial.print(strStatus);
  
  if (openTime.length() > 0) {
    Serial.print(". It was open for " + openTime);
  }
  Serial.println(".");

  // Server Expects updates in the form:
  // http://yourserver.com/yourdoorstatusfolder/updateDoorStatus.php?secret=seomethingsecret&doorID=1&status=CLOSED&opentime=3:44

  // This will send the request to the server
  client.print(String("GET ") + url + "&doorID=" + doorID + "&status=" + strStatus + "&opentime=" + openTime + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" + 
             "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

