Documentation on languages: [english](README.md), [русский](README-RU.md).

# What is it?

This is Arduino library for controlling seven-segment indicators blocks and LED matrixes based on MAX7219. Perhaps it works with other devices.


# How to insall

1. Download. Click "Download ZIP" on the right side to do it.
2. Install. In Arduino IDE: top menu — Sketch — Import Library... — Add Library — choose downloaded ZIP or library directory.
3. Import. In Arduino IDE: top menu — Sketch — Import Library... — Segment7Display.


# Examples

#### Initialisation

	#include <Segment7Display.h>
	#define PIN_DIN 2	// Data pin
	#define PIN_CLK 3	// Clock pin
	#define PIN_CS 4	// Strobe pin
	Segment7Display display(PIN_DIN, PIN_CLK, PIN_CS);

	void setup() {
		// For proper display operation this method showld be called after Arduino initialisation
		display.reset();
	}

#### Set indicator state
	
	// "Tabby" state is set to second right indicator
	display.printData(1, 0b10101010);

#### Symbol printing

	display.printChar(1, 't');

#### Text string printing
	
	display.printString("HELLO");

#### Float number printing
	
	// Only 1 digit left after decimal point
	display.printFloat(123.456, 1);


# API

## Methods

#### Constructor

	Segment7Display(int pData, int pClock, int pStrobe)

Parameters:
* `int pData` Number of data pin (DIN)
* `int pClock` Number of clock pin (CLK)
* `int pStrobe` Number of strobing pin (CS)


#### reset

	void reset(float brightness = 1)

Reset all chip parameters to normal state. This method should be called in or after executing of Arduino `setup` function. Otherwise there is great possibility of indicators incorrect behaviour.

Parameters:
* `float brightness` LEDs brightness. From 0 to 1. Default: maximum.


#### setTest

	void setTest(boolean on)

Turns on/off segments test mode. In test mode each LED legment is turned on with maximum brightness.

Parameters:
* `boolean on` true — on, false — off


#### setDigitsAmount

	void setDigitsAmount(int amount)

Sets amount of used indicators (digits).
	
Parameters:
* `amount` Amount of indicators. From 1 (only right one) to 8 (all).


#### setDecode

	void setDecode(boolean on)

Turns on/off chip decode mode.

In decode mode symbols numbers are transfered (not LEDs state). Number is set by 4 right bits. Symbols are: `0 1 2 3 4 5 6 7 8 9 - E H L P (empty)`. 8th right bit sets state of decimal point.

With decode mode off LEDs state is set by 8 right bits. Each bit matches it's segment (LED). Segments are set in this order: DP A B C D E F G.

Parameters:
* `boolean on` true — on, false — off


#### setShutdown

	void setShutdown(boolean on)

Turns on/off sleep mode.

Parameters:
* `boolean on` true — on, false — off


#### setBrightness

	void setBrightness(float brightness)

Set LEDs brightness. 

Parameters:
* `float brightness` LEDs brightness. From 0 to 1.


#### clear

	void clear()

Turns all LEDs off.


#### printByte

	void printByte(byte digit, byte data)

Sets indicator state. Which segments (LEDs) are turned on depends on whether decode mode is on.

With decode mode off LEDs state is set by bits. Each bit matches it's segment (LED). Segments are set in this order: DP A B C D E F G.

In decode mode symbols numbers are transfered (not LEDs state). Number is set by 4 right bits. Symbols are: `0 1 2 3 4 5 6 7 8 9 - E H L P (empty)`. 8th right bit sets state of decimal point.

Parameters:
* `byte digit` Indicator number. From 0 (right) to 7 (left).
* `byte data` State. With decode mode off 0 means clear.


#### printChar

	void printChar(byte digit, char symbol, boolean decimalPoint = false)

Prints symbol in segment. Displayable symbols: `0 1 2 3 4 5 6 7 8 9 A b C c d E F G H h I i J j L l n O o p q r S t U u Y y Z . -`, other symbols are replaced by space. Letters that are not in that list but available in other case are replaced by other case. On exetuting of this method decode mode is turned off.

Parameters:
* `byte digit` Indicator number. From 0 (right) to 7 (left).
* `char symbol` Symbol to print
* `boolean decimalPoint` Whether to print decimal point. Default: no.


#### printString

	void printString(String str, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1)

Prints text string. Only symbols listed in [printChar](#printchar) method documentation are displayed, other are replaced by space. On exetuting of this method decode mode is turned off.

Parameters:
* `String str` String to print
* `byte align` String align. Possible variants are listen in static constants [ALIGN_*](#align_) of this class. Default: right side.
* `boolean clearExcess` Whether to clear indicators not participating in printing of current text string. Default: yes.
* `int offset` Offset from alinging side. Measures in amount of indicators. Default: no offset.
* `int maxLength` Maximum text string length. If below 0 there is no constraint. Default: no constraint.


#### printFloat

	void printFloat(float num, unsigned char precision = 2, boolean E = false, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1)

Prints float number. On exetuting of this method decode mode is turned off.

Parameters:
* `float num` Number to print
* `unsigned char precision` Amount of digits after decimal point. Negative values are allowed. Default: 2.
* `boolean E` true — print in exponent form; false — print in usual form. Default: usual.
* `byte align` String align. Possible variants are listen in static constants [ALIGN_*](#align_) of this class. Default: right side.
* `boolean clearExcess` Whether to clear indicators not participating in printing of current text string. Default: yes.
* `int offset` Offset from alinging side. Measures in amount of indicators. Default: no offset.
* `int maxLength` Maximum text string length. If below 0 there is no constraint. Default: no constraint.


## Static constants

### ALIGN_*

Used in to point text alignment in methods [printString](#printstring) and [printFloat](#printfloat).

* `ALIGN_LEFT` Text string alignes to left side. Excess symbols are cut from right side.
* `ALIGN_RIGHT` Text string alignes to right side. Excess symbols are cut from left side.