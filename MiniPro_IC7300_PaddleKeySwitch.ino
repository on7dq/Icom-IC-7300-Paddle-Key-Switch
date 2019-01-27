// IC7300 Paddle <-> Straight Key switch
// by Luc Decroos - ON7DQ/KF0CR
// 20170105
// 20170201 added code for  : hold button sets PWR to 10W or 100W

#include <SoftwareSerial.h>  // for comms to IC7300

// IC7300 Serial comms
#define BAUD_RATE 9600       // CI-V speed

// serial connection
// RX = IC7300 to Arduino  : to pin 6 via resistor 4k7 (not used now)
// TX = Arduino to IC7300  : direct to pin 7, with pull up 10k to Vcc (5V)
SoftwareSerial mySerial(6, 7); // (RX, TX)

int keyPin = 8;        // key button to GND
int paddlePin = 9;     // paddle button to GND
int ledPin    = 13;     // internal LED
int count = 0;          // counter for press time
int longTime = 500;     // limit for short/long press
boolean paddle = true; // start in paddle mode

void setup()
{

  // connect to IC7300
  mySerial.begin(BAUD_RATE);
  //  mySerial.write(0x00); // send any initialization command if needed

  pinMode(keyPin, INPUT);
  //digitalWrite(keyPin, HIGH); // turn on internal pull-up on the inputPin
  // not used for Nintendo controller

  pinMode(paddlePin, INPUT);
  //digitalWrite(paddlePin, HIGH); // turn on internal pull-up on the inputPin

  pinMode(ledPin, OUTPUT);
}

void loop()
{
  count = 0;
  while (digitalRead(keyPin) == LOW) {
    count++;
    delay(1);
  }
  if (count > 50 && count < longTime) {
    // set rig to straight key input
    blink();
    paddle = false;

    mySerial.flush();
    mySerial.write(0xFE);
    mySerial.write(0xFE);
    mySerial.write(0x94); // default radio address
    mySerial.write(0xE0);
    mySerial.write(0x1A);
    mySerial.write(0x05);
    mySerial.write(0x01);
    mySerial.write(0x64);
    mySerial.write((byte)00);
    mySerial.write(0xFD);
  }
  if (count > longTime) {
    // set PWR to 10W
    blink();
    mySerial.flush();
    mySerial.write(0xFE);
    mySerial.write(0xFE);
    mySerial.write(0x94); // default radio address
    mySerial.write(0xE0);
    mySerial.write(0x14);
    mySerial.write(0x0A);
    mySerial.write((byte)00);
    mySerial.write(0x28);
    mySerial.write(0xFD);
  }

  count = 0;
  while (digitalRead(paddlePin) == LOW) {
    count++;
    delay(1);
  }
  if (count > 50 && count < longTime) {
    // set rig to paddle input
    paddle = true;

    mySerial.flush();
    mySerial.write(0xFE);
    mySerial.write(0xFE);
    mySerial.write(0x94); // default radio address
    mySerial.write(0xE0);
    mySerial.write(0x1A);
    mySerial.write(0x05);
    mySerial.write(0x01);
    mySerial.write(0x64);
    mySerial.write(0x02);
    mySerial.write(0xFD);
  }
  if (count > longTime) {
    // set PWR to 100W
    mySerial.flush();
    mySerial.write(0xFE);
    mySerial.write(0xFE);
    mySerial.write(0x94); // default radio address
    mySerial.write(0xE0);
    mySerial.write(0x14);
    mySerial.write(0x0A);
    mySerial.write(0x02);
    mySerial.write(0x55);
    mySerial.write(0xFD);
  }
  delay(100);
}

void blink() {
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);
}

