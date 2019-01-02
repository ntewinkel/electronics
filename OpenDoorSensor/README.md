# Open Door Sensor
This project will monitor the status of a door for you. Intended to be used as a reminder to not leave the garage door open, this can be adapted for other purposes too.
This project originally started as a sump or septic pump monitor, with only minor changes in wording, and a light sensor instead of a reed switch.

Basic Workings:
The ESP-based Wemos board connects to your own WiFi router, and then waits for the switch position to change.
Whenever the switch opens or closes, the Wemos calls a php script on the server to record it.
When the switch opens (door is open), it also starts counting so that when the door closes (switch closes) it can include a time-open number. It also tracks if the door has been open too long, and starts sending warnings if that happens.
Adjust warning time threshold and repeats to suit by changing the defined values near the top of the Arduino ino file.

Information you need to provide to use this project:
* Update the Arduino ino file to:
    * set your WiFi router name and password
    * set the server name and path to your updateDoorStatus.php script, and your own secret password
* Update the updateDoorStatus.php script to:
    * set your own secret password
    * set the email address for the recipient. In my case I use email-to-text, so I receive the updates as text messages.

Parts:
* 1 Wemos D1 Mini or compatible, or similar ESP8266 based Arduino dev board.
* 1 reed switch or any switch you can place in a way that causes the switch to be closed when the door is completely shut, and open otherwise.
* your own server that supports PHP scripts

Please feel free to use it as you wish (thus the MIT license)
