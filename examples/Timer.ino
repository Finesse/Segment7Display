/*
Выводит количество секунд, прошедших с начала работы
*/

#include <Segment7Display.h>

#define DIN 4
#define LOAD 3
#define CLK 2

Segment7Display led(DIN, CLK, LOAD);

void setup() {
  led.reset();
}

void loop() {
  led.printFloat((float)millis() / 1000, 1);
  delay(10);
}
