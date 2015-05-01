/*
  Prints serial port messages.
  Messages are separated by new line.
*/

#include <Segment7Display.h>

#define DIN 4
#define LOAD 3
#define CLK 2

Segment7Display led(DIN, CLK, LOAD);
String message;

void setup() {
  Serial.begin(9600);
  led.reset();
  led.printString("HELLO");
}

void loop() {
  char income;

  while(Serial.available()) {
    income = Serial.read();
      
    if(income == '\n' || income == '\r') {
      led.printString(message);
      message = "";
    } else {
      message += income;
    }
  }
}
