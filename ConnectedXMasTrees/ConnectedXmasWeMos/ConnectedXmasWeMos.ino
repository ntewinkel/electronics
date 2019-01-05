/*
 * ConnectedXmas
 * 
 * Connected Xmas Trees
 * 
 * Using MQTT, motion sensed at one tree will light the other tree
 * 
 * Motion sensed also lights the tip of the local tree, to give indication to the local user.
 * 
 * free service: https://www.hivemq.com/try-out/
  http://www.mqtt-dashboard.com/
  Use the browser based client for testing:
    http://www.hivemq.com/demos/websocket-client/

  On the browser client, remember to:
  1) connect (use defaults)
  2) Subscribe to the topic: ConnectedXmasTreeTopic

  From browser client, can send a 1 to turn on the tree, 0 to turn it off
    
  PubSubClient for Arduino - use Arduino Package Installer. -> see examples that are included in PubSubClient.
  Also see https://www.baldengineer.com/mqtt-tutorial.html
 * 
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi connection
const char* ssid     = "yourWiFiName";  // replace with your own WiFi network name
const char* password = "YourWiFiPassword"; // replace with your own WiFi network password

// MQTT server
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "ConnectedXmasTreeTopic"; // Make this unique in case others are doing the same - maybe add your name to the end.

char mqtt_message[50];

const char myTreeID = '1';     // switch these two numbers for the second tree.
const char otherTreeID = '2';  //  ie, one WeMos gets programmed with myTreeID = 1 and otherTreeID = 2, the other WeMos gets 2 and 1.

// Use WiFiClient class to create TCP connections
WiFiClient espClient;
PubSubClient client(espClient);

// For Wemos D1 Mini, the built-in LED is D4
#define MOTION_SENSOR D3  // D8 has built-in 10k pull-down, D3 has pull-up
#define LED_TREETOP D1
#define LED_TREE    D2

#define LED_ON  HIGH
#define LED_OFF LOW

#define SECOND 1000ul
#define MINUTE (60ul * SECOND)

#define TREE_ON_TIME 10*SECOND // 5*MINUTE
#define MIN_TIME_BETWEEN_MESSAGES 5*SECOND

unsigned long timeThisTreeTurnedOn;
unsigned long timeOtherTreeTurnedOn;

bool thisTreeIsOn = false;
bool otherTreeIsOn = false;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.println("\n\nStarting...\n");
  
  pinMode(LED_TREETOP, OUTPUT);
  pinMode(LED_TREE, OUTPUT);

  digitalWrite(LED_TREETOP, LED_OFF);
  digitalWrite(LED_TREE, LED_OFF);

  pinMode(MOTION_SENSOR, INPUT);  // use a 10k external resistor to ground

  connectWiFi();

  Serial.println("Setting up MQTT connection");
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);

  Serial.println("Setup is complete.\n");
}

void connectWiFi() {
  delay(10);

  Serial.print("\n\nConnecting to WiFi: ");
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
//  Serial.print("Netmask: ");
//  Serial.println(WiFi.subnetMask());
//  Serial.print("Gateway: ");
//  Serial.println(WiFi.gatewayIP());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on this tree if an 1 was received as first character
  if ((char)payload[0] == myTreeID) {
    flashThisTree();
    
    if (length == 1) {
      lightThisTree();
    }
  }

}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ConnectedXmasTreeClient-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // this will cause the other tree to flash
      snprintf (mqtt_message, 50, "%c Hello world!", otherTreeID);
      client.publish(mqtt_topic, mqtt_message);
      // ... and resubscribe
      client.subscribe(mqtt_topic);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      // tree top flashes when waiting to reconnect
      for (int i=0; i<10; i++) {
        digitalWrite(LED_TREETOP, LED_ON);
        delay(250);
        digitalWrite(LED_TREETOP, LED_OFF);
        delay(250);
      }
    }
  }
}

void loop() {
  
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop();

  // check for motion, unless motion was already detected recently
  if (millis() - timeOtherTreeTurnedOn > MIN_TIME_BETWEEN_MESSAGES) {
    // instead of button, will have motion sensor
    if (digitalRead(MOTION_SENSOR) == HIGH) {
      Serial.println("Motion detected!");
      lightOtherTree();
    }
  }

  if (otherTreeIsOn && millis() - timeOtherTreeTurnedOn > TREE_ON_TIME) {
    // no message needed - other tree turns off at same kind of interval
    // turn off tree top indicator light
    digitalWrite(LED_TREETOP, LED_OFF);
    otherTreeIsOn = false;
  }

  if (thisTreeIsOn && millis() - timeThisTreeTurnedOn > TREE_ON_TIME) {
    digitalWrite(LED_TREE, LED_OFF);
    thisTreeIsOn = false;
  }
}

void lightOtherTree() {
  char msg[2];
  msg[0] = otherTreeID;
  msg[1] = '\0';
  digitalWrite(LED_TREETOP, LED_ON);  // turn on local tree top light. Green for testing
  Serial.println("Sending message to light the other tree");
  client.publish(mqtt_topic, msg);
  otherTreeIsOn = true;
  timeOtherTreeTurnedOn = millis();
}

void lightThisTree() {
  // flash the tree
  digitalWrite(LED_TREE, LED_ON);
  delay(200);
  digitalWrite(LED_TREE, LED_OFF);
  delay(200);

  digitalWrite(LED_TREE, LED_ON);
  thisTreeIsOn = true;
  timeThisTreeTurnedOn = millis();
}

void flashThisTree() {
  for (int i=0; i<7; i++) {
    digitalWrite(LED_TREE, LED_ON);
    delay(100);
    digitalWrite(LED_TREE, LED_OFF);
    delay(100);
  }
}

