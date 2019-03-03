# Multi Sensor Logging
This project uses an IoT enabled board to regularly read a temperature and report it to a server.
I've been using mostly Wemos D1 Minis for my more recent projects, using the Arduino IDE, but any other Internet connected device that can call a web based URL would work.

The server side is handled via the php scripts included here.

Please feel free to use it as you wish (thus the MIT license)

This project is very similar to the ESP-01 project, but it has been expanded to allow for multiple sensor values to be logged and graphed together. It also removes the mostly unused single-value storage and display of the previous version.

The process is as follows:

1. The IoT device reads from one or more sensors. Note that the graph kind of assumes all values are of the same type - for now they should all be temperatures. At this point the graph Y axis still assumes Temperature in Celcius.

2. The IoT device calls logMultiSensorValues.php in the following format:
http://localhost/sensors/logMultiSensorValues?sensorID=007&values={%22inside%20temp%22:22.5,%22outside%20temp%22:15.1}
Quotes and spaces should be url-encoded. There is no real limit to how many temperatures can be sent in the list.

3. The php script (server side) stores the data into a .csv file. If the file does not exist, it writes the first line of the file with the titles provided in the values array that was sent by the IoT device - ie, the IoT device labels the graph lines

4. To display the graph, open this URL in a browser:
http://localhost/sensors/sensorgraph_multi.html?sensorID=007
Adjust the sensor ID to be unique for each IoT device that is sending information. In my example, sensor 007 actually has two physical temperature sensors (inside and outside), and the one sensorID is used to group them into one log file.

Notes:
* For testing I have a local webserver, ie "localhost". You will need to replace "localhost" in the examples above with your domain name.
* As this is not mission critical, there is no real security or encryption being used. You might want to use a semi-secret folder instead of just "sensors".
* To avoid annoying people creating thousands of log files on your system, the script only stores values for sensors named 000 through 009.
