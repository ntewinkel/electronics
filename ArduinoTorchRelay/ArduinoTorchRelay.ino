//
// Arduino Birthday - pass the torch!
//
// Light sensor triggers LED light on after 1 second
//

//#define WEMOS
//#define DIGISPARK

// We might be able to use LED_BUILTIN for most
#ifdef WEMOS
int LDR = A0;
int ledPin = 12; // to keep the green theme
#elif defined DIGISPARK
int LDR = 1; // Analog P2 Digispark. Analog inputs use pin number minus 1
int ledPin = 1; // Digital P1 Digispark. Also the built-in LED
#else
int LDR = A0;      // select the input pin for the LDR. Digispark use 2 = P2, but do analogRead(1) to read it
int ledPin = 13;
#endif

/* analogread notes for digistump:     
 *  
    sensorValue = analogRead(1); //Read P2
    //To set to input: pinMode(2, INPUT);
    //THIS IS P2, P2 is analog input 1, so when you are using analog read, you refer to it as 1.
 */

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);
  
#ifdef DIGISPARK
  pinMode(LDR+1, INPUT_PULLUP);  // Digispark - pinMode is done on actual pin number
#else
  pinMode(LDR, INPUT_PULLUP);
#endif

  digitalWrite(ledPin, LOW);   // turn the ledPin off

//  Serial.begin(115200);
//  Serial.println("Starting...");

}

void loop() {
  // put your main code here, to run repeatedly:

  int val = analogRead(LDR);       // read the value from the sensor

  // LDR read of < 500 = green LED ON, higher value when off, depending on room light
//  Serial.print("val = ");
//  Serial.println(val);

#ifdef WEMOS
  val = 1024-val; // invert the value
#endif

  while (val > 500) {
    delay(100); // 10x per second
    val = analogRead(LDR);

#ifdef WEMOS
  val = 1024-val; // invert the value
#endif

//    Serial.print("val = ");
//    Serial.println(val);
  }

  // previous torch-bearer will be on for a second
  delay (1000);

  digitalWrite(ledPin, HIGH);  // turn the ledPin on
  delay (1000);
  digitalWrite(ledPin, LOW);   // turn the ledPin off

}
