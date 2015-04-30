/*
	This is Arduino library for controlling seven-segments indicator blocks and LED matrixes based on MAX7219. Perhaps it works with other devices.

	1.0

	http://github.com/FinesseRus/Segment7Display
*/




#pragma once
#include <Arduino.h>

class Segment7Display {
	public:

		/*
			Static constants used in to point text alignment in methods printString and printFloat.
		*/
		// Text string alignes to left side. Excess symbols are cut from right side.
		static const byte ALIGN_LEFT = 0;
		// Text string alignes to right side. Excess symbols are cut from left side.
		static const byte ALIGN_RIGHT = 1;

		/*
 			Constructor

			Parameters:
			* `int pData` Number of data pin (DIN)
			* `int pClock` Number of clock pin (CLK)
			* `int pStrobe` Number of strobing pin (CS)
		*/
		Segment7Display(int pData, int pClock, int pStrobe);
		
		/*
			Reset all chip parameters to normal state. This method should be called in or after executing of Arduino setup function. Otherwise there is great possibility of indicators incorrect behaviour.

			Parameters:
			* `float brightness` LEDs brightness. From 0 to 1. Default: maximum.
		*/
		void reset(float brightness = 1);

		/*
			Turns on/off segments test mode. In test mode each LED legment is turned on with maximum brightness.

			Parameters:
			* `boolean on` true — on, false — off
		*/
		void setTest(boolean on);

		/*
			Sets amount of used indicators (digits).
				
			Parameters:
			* `amount` Amount of indicators. From 1 (only right one) to 8 (all).
		*/
		void setDigitsAmount(int amount);

		/*
			Turns on/off chip decode mode.

			In decode mode symbols numbers are transfered (not LEDs state). Number is set by 4 right bits. Symbols are: `0 1 2 3 4 5 6 7 8 9 - E H L P (empty)`. 8th right bit sets state of decimal point.

			With decode mode off LEDs state is set by 8 right bits. Each bit matches it's segment (LED). Segments are set in this order: DP A B C D E F G.

			Parameters:
			* `boolean on` true — on, false — off
		*/
		void setDecode(boolean on);

		/*
			Turns on/off sleep mode.

			Parameters:
			* `boolean on` true — on, false — off
		*/
		void setShutdown(boolean on);

		/*
			Set LEDs brightness. 

			Parameters:
			* `float brightness` LEDs brightness. From 0 to 1.
		*/
		void setBrightness(float brightness);

		/*
			Turns all LEDs off.
		*/
		void clear();

		/*
			Sets indicator state. Which segments (LEDs) are turned on depends on whether decode mode is on.

			With decode mode off LEDs state is set by bits. Each bit matches it's segment (LED). Segments are set in this order: DP A B C D E F G.

			In decode mode symbols numbers are transfered (not LEDs state). Number is set by 4 right bits. Symbols are: `0 1 2 3 4 5 6 7 8 9 - E H L P (empty)`. 8th right bit sets state of decimal point.

			Parameters:
			* `byte digit` Indicator number. From 0 (right) to 7 (left).
			* `byte data` State. With decode mode off 0 means clear.
		*/
		void printByte(byte digit, byte data);

		/*
			Prints symbol in segment. Only symbols listed in char2byte method documentation are displayed, other are replaced by space. On exetuting of this method decode mode is turned off.

			Parameters:
			* `byte digit` Indicator number. From 0 (right) to 7 (left).
			* `char symbol` Symbol to print
			* `boolean decimalPoint` Whether to print decimal point. Default: no.
		*/
		void printChar(byte digit, char symbol, boolean decimalPoint = false);

		/*
			Prints text string. Only symbols listed in char2byte method documentation are displayed, other are replaced by space. On exetuting of this method decode mode is turned off.

			Parameters:
			* `String str` String to print
			* `byte align` String align. Possible variants are listen in static constants ALIGN_* of this class. Default: right side.
			* `boolean clearExcess` Whether to clear indicators not participating in printing of current text string. Default: yes.
			* `int offset` Offset from alinging side. Measures in amount of indicators. Default: no offset.
			* `int maxLength` Maximum text string length. If below 0 there is no constraint. Default: no constraint.
		*/
		void printString(String str, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1);

		/*
			Prints float number. On exetuting of this method decode mode is turned off.

			Parameters:
			* `float num` Number to print
			* `unsigned char precision` Amount of digits after decimal point. Negative values are allowed. Default: 2.
			* `boolean E` true — print in exponent form; false — print in usual form. Default: usual.
			* `byte align` String align. Possible variants are listen in static constants ALIGN_* of this class. Default: right side.
			* `boolean clearExcess` Whether to clear indicators not participating in printing of current text string. Default: yes.
			* `int offset` Offset from alinging side. Measures in amount of indicators. Default: no offset.
			* `int maxLength` Maximum text string length. If below 0 there is no constraint. Default: no constraint.
		*/
		void printFloat(float num, unsigned char precision = 2, boolean E = false, byte align = ALIGN_RIGHT, boolean clearExcess = true, int offset = 0, int maxLength = -1);

	protected:
		/*
			Переменные для запоминания пинов. Какие за что отвечают, смотрите в описании Segment7Display.
		*/
		/*
			Pins numbers. What is responsible for what watch in constructor documentation.
		*/
		int pinData, pinClock, pinStrobe;

		// Whether decode mod is on
		boolean decodeModeOn;

		// Table of ASCII symbols matching to 7-segments indicator states
		static const byte charTable[];

		/*
			Prepares package for sending to chip.

			Parameters:
				byte command — Code of command (only 4 right bits are used).
				byte data — Concomitating data.

			Returns: (word) Prepared package.
		*/
		static word preparePackage(byte command, byte data);

		/*
			Sends packages to chip.

			Parameters:
				package — Sended package.
		*/
		void sendPackage(word package);

		/*
			Proportionally converts number (value) from one range (fromMin .. fromMax) to other range (toMin .. toMax). Like Arduino map function but float-firendly.
		*/
		static int mapNormal(float value, int fromMin, int fromMax, int toMin, int toMax);

		/*
			Encodes symbol to indicator state byte. Any symbol can be passed but only those will by encoded: 0 1 2 3 4 5 6 7 8 9 A b C c d E F G H h I i J j L l n O o p q r S t U u Y y Z . -. Letters that are not in that list but available in other case are replaced by other case. Space symbol turns indicator off.

			Parameters:
				char symbol — Symbol to encode.
				boolean decimalPoint — Whether to add decimal point. Default: no.

			Returns: (byte) Encoded byte.
		*/
		static byte char2byte(char symbol, boolean decimalPoint = false);

		/*
			Encodes text string to bytes array for printing on 7-segments indicators. Only symbols listed in char2byte method documentation are displayed, other are replaced by space.

			Parameters:
				String str — String to encode.
				byte* output — Pointer to output buffer for encoded bytes.
				int maxLength — Maximum text string length. If below 0 there is no constraint.
				byte align — String align. Possible variants are listen in static constants ALIGN_* of this class. Default: right side.

			Returns: (unsigned int) Amount of encoded bytes written to output buffer.
		*/
		static unsigned int string2bytes(String str, byte* output, int maxLength, byte align = ALIGN_RIGHT);
};