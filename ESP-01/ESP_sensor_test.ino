/*
  ESP_sensor
  Reads the sensor value and updates the server with it
 
  The server call looks like:
  http://www.mulberryland.com/secretlocation/updateSensorValue.php?sensorID=002&value=77
 */

#include <ESP8266WiFi.h>

const char* ssid     = "mulBerryLand";
const char* password = "secretpasswrd";
 
const char* host = "www.mulberryland.com";
const char* url = "/secretlocation/updateSensorValue.php";

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;

// Set how long to sleep between sensor readings, to save battery life
const int sleepTimeSecs = 10;  // in seconds

// identify this specific sensor with a unique ID
String sensorID = "001";

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
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

  // initialize the digital pin as an output.
  pinMode(2, OUTPUT); // GPIO2 on ESP-01 is the only fully free pin available (GPIO0 should be available too with some extra work)
}

// the loop routine runs over and over again forever:
void loop() {

  for (int sensorValue=0; sensorValue<50; sensorValue++) {
    digitalWrite(2, HIGH);

    // connect to the host
    Serial.println("Making the connection to the host...");
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    String label = "Sending request with new sensorValue: ";
    Serial.println(label + sensorValue);
        
    // This will send the request to the server
    client.print(String("GET ") + url + "?sensorID=" + sensorID + "&value=" + sensorValue +  " HTTP/1.1\r\n" +
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
    
    digitalWrite(2, LOW); 
    delay(1000); // wait for a second

    // sleep until the next reading
//    Serial.println("ESP8266 is sleeping...");
//    ESP.deepSleep(sleepTimeSecs * 1000000);
//    Serial.println("ESP8266 is awake.");
  }
  
  // When done, just blink the LED
  while(true) {
    digitalWrite(2, HIGH);
    delay(1000);               // wait for a second
    digitalWrite(2, LOW); 
    delay(1000);               // wait for a second
  }
}
