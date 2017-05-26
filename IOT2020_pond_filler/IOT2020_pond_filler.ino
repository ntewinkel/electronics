//
// This is a pond filler controller
//
// When the water level gets too low (low water mark input),
//    the relay turns on to activate a sprinkler valve that adds water to the pond
//
// The relay stays on (pond keeps filling up) until the pond is full (high water mark input)
// The relay is also turned off if it has been on too long (15 minutes)
// A warning light goes on if it's been on too long
//
// Inputs will go low when they are in the water
//
// Note that the resistance in water is very high.
// My tap water tested to about 1.1 M-ohm, and the pond water was even higher at 1.6 M-ohm
// This means the internal 20k-ohm resistor won't be sufficient.
// Use something like 4.7 M-ohm resistors to pull-up to 5v  (3 M-ohm or higher will probably work)
//
// Created by Nico teWinkel, (c) April 2017
// MIT license: https://opensource.org/licenses/MIT
// Free to use. If it makes you a millionaire, please buy me a coffee :)
//
// How to program the IOT2020: https://www.rs-online.com/designspark/program-the-iot2000-with-arduino-ide
// Select board "Intel Galileo Gen2"
// Select port that includes name "Intel Galileo"

// NOTE that some relay modules are on when signal is low
#define RELAY_ON HIGH
#define RELAY_OFF LOW

#define MAX_ON_TIME 15 //* 60 // 5 minutes per day of filling is a lot. For testing is set at 15 seconds
#define MILLIS_IN_ONE_DAY 24ul * 60ul * 60ul * 1000ul

#define WARNING_LED 13
#define RELAY 7
#define LOW_WATER 5
#define HIGH_WATER 6

void test_print_time();

unsigned long on_time = 0L;
bool filling = false;
bool on_too_long = false;
bool warning = false;
bool warning_blink = false;
unsigned long start_day;

void setup() {
  pinMode(WARNING_LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LOW_WATER, INPUT);
  pinMode(HIGH_WATER, INPUT);

  digitalWrite(RELAY, RELAY_OFF);
  digitalWrite(WARNING_LED, LOW);

  start_day = millis();

  Serial.begin(9600);
  Serial.println("Starting the Pond Filler program.");

//  test_print_time();
}

void loop() {

  // reset on_time once a day (but do not reset the warning light)
  if ((millis() - start_day) >= MILLIS_IN_ONE_DAY) {
    Serial.println("Starting new day of monitoring.");
    start_day = millis();
    on_too_long = false;
  }
  
  if (warning) {
    // blink the warning LED
    warning_blink = !warning_blink;
    
    if (warning_blink) {
        digitalWrite(WARNING_LED, HIGH);
    }
    else {
        digitalWrite(WARNING_LED, LOW);
    }
  }

  if (filling) {
    on_time++;  // roughly a second has passed
    
    if (on_time > MAX_ON_TIME) {
      digitalWrite(RELAY, RELAY_OFF);
      filling = false;
      on_too_long = true;
      warning = true;
      Serial.println("WARNING! Pond filler has been on too long!");
    }
    // else check if pond is full enough
    else if (digitalRead(HIGH_WATER) == LOW) { // when reading is low, water is high enough
      // when pond is full, or if valve has been on too long, turn off the relay
      digitalWrite(RELAY, RELAY_OFF);
      filling = false;
      Serial.println("Stopping pond filler.");
      Serial.print("Pond filler was on for ");
      print_time(on_time);
    }
  }
  else { // not filling
    
    if (on_too_long) {
      // if it's been on too long, don't start it up again
    }
    else {
      // check if water is too low
      if (digitalRead(LOW_WATER) == HIGH) { // when reading is high, water is too low
        // pond water level is low, turn on the relay
        Serial.println("Starting pond filler.");
        digitalWrite(RELAY, RELAY_ON);
        filling = true;
        on_time = 0L;
      }
    }
  }

  delay(1000); // wait one second
}

void test_print_time() {
  Serial.print("Pond filler was on for ");
  print_time(0);    // 0:00
  print_time(25);   // 0:25
  print_time(90);   // 1:30
  print_time(615);  // 10:15
  print_time(3600); // 1:00:00
  print_time(4215); // 1:10:15
}

void print_time(unsigned long num_secs) {
  int hours = num_secs / 3600;
  int left_over_secs = num_secs % 3600;
  int mins = left_over_secs / 60;
  int secs = left_over_secs % 60;

  if (hours > 0) {
    Serial.print(hours);
    Serial.print(":");
  }
  if (mins < 10) Serial.print("0");
  Serial.print(mins);
  Serial.print(":");
  if (secs < 10) Serial.print("0");
  Serial.println(secs);
}

