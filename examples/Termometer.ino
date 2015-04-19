/*
Выводит на индикаторе температуру.

Схема датчика температуры:

(+E)
  │
 [R]
  ├───(A0)
 [T]
  │
  ¯
 Где [R] — резистор на 10кОм, [T] — термистор B57164-K 103-J (10кОм при 25 градусах).

*/

#include <Segment7Display.h>

#define TERMOMETER_PIN A0
#define DIN 4
#define LOAD 3
#define CLK 2

#define TEMP_AMOUNT 10
Segment7Display led(DIN, CLK, LOAD);

// Чтобы цифры на дисплее постоянно не менялись от флуктуации температуры, сохряняем несколько последних измерений и вычисляем их среднее
float temperatures[TEMP_AMOUNT];

void setup() {
  led.reset();
  
  for(int i = 0; i < TEMP_AMOUNT; ++i)
    temperatures[i] = 0;
}

void loop() {
  // Определение текущей температуры
  float voltage = analogRead(TERMOMETER_PIN) * 5.0 / 1023.0;
  float temperature = 1.0 / (log(voltage / 2.5) / 4300.0 + 1.0 / 298.0) - 273.0;
  
  // Добавление температуры в массив последних температур
  for(int i = TEMP_AMOUNT - 1; i > 0; --i)
    temperatures[i] = temperatures[i - 1];
  temperatures[0] = temperature;
  
  // Вычисление средней температуры
  temperature = 0;
  for(int i = 0; i < TEMP_AMOUNT; ++i)
    temperature += temperatures[i];
  temperature /= TEMP_AMOUNT;
  
  // Вывод температуры на экран
  led.printFloat(temperature, 1);
  
  delay(50);
}