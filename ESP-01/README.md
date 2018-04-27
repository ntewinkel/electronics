# ESP-01
This project uses an ESP-01 or any other ESP-based board to regularly read a temperature and report it to a server.

The server side is handled via the php scripts included here.

Please feel free to use it as you wish (thus the MIT license)

If you find it helpful and/or it makes you rich, please buy me a coffee :)

To see a full description of this project and how it works, please see my blog entry on element14.com:
https://www.element14.com/community/community/project14/mixing-electronics-water/blog/2018/04/13/remote-temperature-monitoring

Here is a quick summary of the files in this project:


Firmware for the ESP-01, for the Arduino IDE: ESP_temp_sensor.ino

Testing firmware for the ESP-01, for the Arduino IDE: ESP_sensor_test.ino which simply loops through, sending values to the server.

I've included a text summary of the connections required to program it: "ESP-01 connections.rtf"

The ESP-01 calls this script on the server: updateSensorValue.php

That script stores the values, with timestamps, into a file on the server named sensorValues.json (included as an example, but is created by the server)

That same script also logs a history of values+timestamps in a separate log file, named log_001.csv (where 001 is the sensor name). This script restricts logging to sensors named "000" through "009" to avoid abuse, as each creates a new file. I've included log_001.csv as an example. It's in proper .csv format so you can easily open/view it with spreadsheet software like OpenOffice Calc.

I've also included a no-logging version of that script: updateSensorValue_no_logging.php. As I build and test things, I usually start with a basic starting point and then add features as things progress.

The sensorsSummary.php script shows a basic summary of the sensor values in an html table. You can view it from a browser, just like any other webpage.

And this is the HTML page that shows a graph for a sensor: sensorgraph.html

That html page calls a helper script named getData.php that translates the stored data from log_001.csv into a format used by the Google functions to create the graph. Note that log_001.csv is currently hardcoded. I want to one day update the script to handle multiple log files, to allow sensorgraph.html to be called with a parameter that is the sensor name.
