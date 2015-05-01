/*
  Prints temperature in Celsius degrees.

  Temperature sensor circuit:

  (+E)
    │
   [R]
    ├───(A0)
   [T]
    │
    ¯
   Where [R] — resistor on 10kOm, [T] — termistor B57164-K 103-J (10kOm on 25°C).
*/

#include <Segment7Display.h>

#define TERMOMETER_PIN A0
#define DIN 4
#define LOAD 3
#define CLK 2

#define TEMP_AMOUNT 10
Segment7Display led(DIN, CLK, LOAD);

// Saving few last temperature measuring to smooth temperature wobbling
float temperatures[TEMP_AMOUNT];

void setup() {
  led.reset();
  
  for(int i = 0; i < TEMP_AMOUNT; ++i)
    temperatures[i] = 0;
}

void loop() {
  // Getting current temperature
  float voltage = analogRead(TERMOMETER_PIN) * 5.0 / 1023.0;
  float temperature = 1.0 / (log(voltage / 2.5) / 4300.0 + 1.0 / 298.0) - 273.0;
  
  // Adding current temperature to last temperatures list
  for(int i = TEMP_AMOUNT - 1; i > 0; --i)
    temperatures[i] = temperatures[i - 1];
  temperatures[0] = temperature;
  
  // Calculating avarage temperature
  temperature = 0;
  for(int i = 0; i < TEMP_AMOUNT; ++i)
    temperature += temperatures[i];
  temperature /= TEMP_AMOUNT;
  
  // Printing temperature
  led.printFloat(temperature, 1);
  
  delay(50);
}