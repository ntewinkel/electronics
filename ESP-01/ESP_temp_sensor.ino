/*
  ESP_sensor
  Reads a temperature sensor value and updates the server with it
 
  The server call looks like:
  http://www.mulberryland.com/secretlocation/updateSensorValueWithLogging.php?sensorID=002&value=77
 */

#include <ESP8266WiFi.h>
#include <OneWire.h>  
#include <DallasTemperature.h>  

const char* ssid     = "mulBerryLand";  // replace with your own WiFi network name
const char* password = "secretpasswrd"; // replace with your own WiFi network password
 
const char* host = "www.mulberryland.com";
const char* url = "/secretlocation/updateSensorValue.php"; // replace secretlocation with your path

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;

// Temperature sensor data wire connects to GPIO2
#define ONE_WIRE_BUS 2  
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define SECOND 1000
#define MINUTE (60 * SECOND)

// Set how long to sleep between sensor readings, to save battery life
const int sleepTimeSecs = 10;  // in seconds

// identify this specific sensor with a unique ID (for the server to tell them apart)
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

  // start the temperature sensor library
  sensors.begin();  

}

void updateServer(float temperature) {
  
  // connect to the host
  Serial.println("Making the connection to the host...");
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String label = "Sending request with new temperature: ";
  Serial.println(label + temperature);
      
  // This will send the request to the server
  client.print(String("GET ") + url + "?sensorID=" + sensorID + "&value=" + temperature +  " HTTP/1.1\r\n" +
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

// the loop routine runs over and over again forever:
void loop() {

  // read the sensor(s) - oneWire can have multiple sensors on the same data line
  sensors.requestTemperatures();

  float newTemperature = sensors.getTempCByIndex(0);

  updateServer(newTemperature);
  
  delay(MINUTE); // wait a minute before the next reading

    // sleep until the next reading
// sleep has been disabled for now, and we are using delay() above instead
// sleep would allow much more efficient use of power for when you're running on solar or battery
//    Serial.println("ESP8266 is sleeping...");
//    ESP.deepSleep(sleepTimeSecs * 1000000);
//    Serial.println("ESP8266 is awake.");
  
}
