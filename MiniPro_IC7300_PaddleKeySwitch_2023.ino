// IC7300 Mini Controller in an old Nintendo Game controller ;-)
// see picture on https://nl.m.wikipedia.org/wiki/Bestand:Nintendo-Entertainment-System-NES-Controller-FL.jpg
// by Luc Decroos - ON7DQ (KF0CR)
// 20170105 first version with 2 buttons to switch key type : paddle or straight key
// 20170201 added code for long press on the 2 buttons : sets PWR to 10W or 100W
// 20230201 added more functions, using the UP/DN/LEFT/RIGHT/SELECT/START buttons, changed LOW PWR to 5W

/* note to self (because I always forget!):
    my board settings :  Arduino Pro or Pro Mini , ATmega168 (5V, 16 MHz), COM2
    to program : hold the RESET button while compiling, and release it when upload starts
*/

#include <SoftwareSerial.h>  // for comms to IC7300
#define adr 0x94             // IC-7300 address
// IC7300 Serial comms
#define BAUD_RATE 9600       // CI-V speed

// serial connection from CI-V (3.5 mmm stereo jack, using only GND and TIP)
// RX = IC7300 to Arduino  : from TIP to pin D6 via a 4k7 resistor (not used now)
// TX = Arduino to IC7300  : from D7 to TIP, via 1N4148 diode (cathode to D7 side)

SoftwareSerial mySerial(6, 7); // (RX, TX)

// define inputs          <Nintendo name>, <function>
int keyPin    = 8;      // B button, sets straight key, long press = low power
int paddlePin = 9;      // A button, sets paddle ,      long press = high power
int startPin  = 10;     // START , sets USB out to AF
int selectPin = 11;     // SELECT, sets USB out to IF (12 kHz)

int upPin     = 2;      // UP    , sends CW memory 1
int rightPin  = 3;      // RIGHT , sends CW memory 2
int downPin   = 4;      // DOWN  , sends CW memory 3
int leftPin   = 5;      // LEFT  , sends CW memory 4
int ledPin    = 13;     // internal LED

boolean startButton  = false;
boolean selectButton = false;
boolean upButton     = false;
boolean rightButton  = false;
boolean downButton   = false;
boolean leftButton   = false;

int count = 0;              // counter for press time
int longTime = 500;         // limit for short/long press
boolean keyPressed = false; // for the 6 single press buttons
boolean paddle     = true;  // start in paddle mode
boolean af         = true;  // start with USB output = AF

void setup()
{
  // connect to IC7300
  mySerial.begin(BAUD_RATE);

  // initialize pins
  pinMode(keyPin, INPUT_PULLUP);
  pinMode(paddlePin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);
}

void loop()
{
  count = 0;
  keyPressed = checkButtons();
  if (!keyPressed) { // process the 2-function buttons
    while (digitalRead(keyPin) == LOW) {
      count++;
      delay(1);
    }
    if (count > 50 && count < longTime) {
      // set rig to straight key input
      paddle = false;
      start();
      // command 1A 05 164 00 = straight key
      mySerial.write(0x1A);
      mySerial.write(0x05);
      mySerial.write(0x01);
      mySerial.write(0x64);
      mySerial.write((byte)00);
      stop();
    }
    if (count > longTime) {
      // set PWR to 5W
      start();
      mySerial.write(0x14);
      mySerial.write(0x0A);
      mySerial.write((byte)00);
      mySerial.write(0x18);
      mySerial.write(0xFD);
      stop();
    }

    count = 0;
    while (digitalRead(paddlePin) == LOW) {
      count++;
      delay(1);
    }
    if (count > 50 && count < longTime) {
      // set rig to paddle input
      paddle = true;
      start();
      mySerial.write(0x1A);
      mySerial.write(0x05);
      mySerial.write(0x01);
      mySerial.write(0x64);
      mySerial.write(0x02);
      stop();
    }
    if (count > longTime) {
      // set PWR to 100W
      start();
      mySerial.write(0x14);
      mySerial.write(0x0A);
      mySerial.write(0x02);
      mySerial.write(0x55);
      stop();
    }
  }
  else {
    // process the single press buttons
    if (startButton) { // set USB output to AF
      start();
      mySerial.write(0x1A);
      mySerial.write(0x05);
      mySerial.write((byte)00);
      mySerial.write(0x59);
      mySerial.write((byte)00);
      stop();
    }
    if (selectButton) { // set USB output to IF
      start();
      mySerial.write(0x1A);
      mySerial.write(0x05);
      mySerial.write((byte)00);
      mySerial.write(0x59);
      mySerial.write(0x01);
      stop();
    }
    if (upButton) { // sends keyer memory 1
      start();
      mySerial.write(0x28);
      mySerial.write((byte)00);
      mySerial.write(0x01);
      stop();
    }
    if (rightButton) { // mem 2
      start();
      mySerial.write(0x28);
      mySerial.write((byte)00);
      mySerial.write(0x02);
      stop();
    }
    if (downButton) { // mem 3
      start();
      mySerial.write(0x28);
      mySerial.write((byte)00);
      mySerial.write(0x03);
      stop();
    }
    if (leftButton) { // mem 4
      start();
      mySerial.write(0x28);
      mySerial.write((byte)00);
      mySerial.write(0x04);
      stop();
    }

  }
  delay(100);
}

// ***************** functions *********************

boolean checkButtons() { // read the single press buttons
  boolean b1, b2, b3, b4, b5, b6, ba = false;

  startButton = false;
  selectButton = false;
  upButton = false;
  rightButton = false;
  downButton = false;
  leftButton = false;

  if (b1 = !digitalRead(startPin))  ba = true;
  if (b2 = !digitalRead(selectPin)) ba = true;
  if (b3 = !digitalRead(upPin))     ba = true;
  if (b4 = !digitalRead(rightPin))  ba = true;
  if (b5 = !digitalRead(downPin))   ba = true;
  if (b6 = !digitalRead(leftPin))   ba = true;

  if (ba) {
    delay(20);
    if (b1 && !digitalRead(startPin)) startButton = true;
    if (b2 && !digitalRead(selectPin)) selectButton = true;
    if (b3 && !digitalRead(upPin)) upButton = true;
    if (b4 && !digitalRead(rightPin)) rightButton = true;
    if (b5 && !digitalRead(downPin)) downButton = true;
    if (b6 && !digitalRead(leftPin)) leftButton = true;
  }
  return ba;
}

void start() {          // send bytes common to all commands
  blink();
  mySerial.flush();
  mySerial.write(0xFE);
  mySerial.write(0xFE);
  mySerial.write(adr); // radio address
  mySerial.write(0xE0);
}

void stop() {           // send the stop bytes
  mySerial.write(0xFD); // stop
}

void blink() {          // confirm the command by blinking the LED on pin D13
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);
}
