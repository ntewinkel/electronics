//
// test water probes
//
// Inputs will go low when they are in the water
//
// Note that the resistance in water is very high.
// My tap water tested to about 1.1 M-ohm, and the pond water was even higher at 1.6 M-ohm
// This means the internal 20k-ohm pull-up resistor won't be sufficient.
// Use something like 4.7 M-ohm resistors to pull-up to 5v  (3 M-ohm or higher will probably work)
//
// Created by Nico teWinkel, (c) April 2017
// MIT license: https://opensource.org/licenses/MIT
//

#define LOW_WATER 5
#define HIGH_WATER 6


void setup() {
  // put your setup code here, to run once:
  pinMode(LOW_WATER, INPUT);
  pinMode(HIGH_WATER, INPUT);

  Serial.begin(9600);
  Serial.println("Starting the Pond Filler.");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(LOW_WATER) == LOW) {
    Serial.println("Water is OVER LOW water point");
  }
  else {
    Serial.println("Water is UNDER LOW water point");
  }

  if (digitalRead(HIGH_WATER) == LOW) {
    Serial.println("Water is OVER HIGH water point");
  }
  else {
    Serial.println("Water is UNDER HIGH water point");    
  }
  
  delay(3000); // wait one second

}
