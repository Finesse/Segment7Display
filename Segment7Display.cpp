#include <Segment7Display.h>

#define CHARS_AMOUNT 128
#define DIGITS_AMOUNT 8
#define BRIGHTNESS_MIN 0x0
#define BRIGHTNESS_MAX 0xf





const byte Segment7Display::charTable[] = {
	0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000,
	0b1111111, 0b1111011, 0b1110111, 0b0011111, 0b0001101, 0b0111101, 0b1001111, 0b1000111,
	0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000001, 0b0000000, 0b0000000,
	0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000,
	0b1111111, 0b1111011, 0b0000000, 0b0000000, 0b0000000, 0b0001001, 0b0000000, 0b0000000,
	0b0000000, 0b1110111, 0b0011111, 0b1001110, 0b0111101, 0b1001111, 0b1000111, 0b1011110,
	0b0110111, 0b0110000, 0b0111000, 0b0000000, 0b0001110, 0b0000000, 0b0010101, 0b1111110,
	0b1100111, 0b1110011, 0b0000101, 0b1011011, 0b0001111, 0b0111110, 0b0000000, 0b0000000,
	0b0000000, 0b0110011, 0b1011011, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0001000,
	0b0000000, 0b1110111, 0b0011111, 0b0001101, 0b0111101, 0b1001111, 0b1000111, 0b1011110,
	0b0010111, 0b0010000, 0b0011000, 0b0000000, 0b0000110, 0b0000000, 0b0010101, 0b0011101,
	0b1100111, 0b1110011, 0b0000101, 0b1011011, 0b0001111, 0b0011100, 0b0000000, 0b0000000,
	0b0000000, 0b0111011, 0b1011011, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000
};



Segment7Display::Segment7Display(int pData, int pClock, int pStrobe) {
	pinData = pData;
	pinClock = pClock;
	pinStrobe = pStrobe;
}



void Segment7Display::reset(float brightness) {
	pinMode(pinData, OUTPUT);
	pinMode(pinClock, OUTPUT);
	pinMode(pinStrobe, OUTPUT);

	setTest(false);
	setDigitsAmount(DIGITS_AMOUNT);
	setDecode(false);
	setShutdown(false);
	setBrightness(brightness);
	clear();
}



void Segment7Display::setTest(boolean on) {
	sendPacket(preparePacket(0b1111, on));
}



void Segment7Display::setDigitsAmount(int amount) {
	if(amount < 1)
		amount = 1;
	else if(amount > DIGITS_AMOUNT)
		amount = DIGITS_AMOUNT;
	sendPacket(preparePacket(0b1011, amount - 1));
}



void Segment7Display::setDecode(boolean on) {
	sendPacket(preparePacket(0b1001, on));
	decodeModeOn = on;
}



void Segment7Display::setShutdown(boolean on) {
	sendPacket(preparePacket(0b1100, !on));
}



void Segment7Display::setBrightness(float brightness) {
	if(brightness < 0)
		brightness = 0;
	else if(brightness > 1)
		brightness = 1;
	sendPacket(preparePacket(0b1010, mapNormal(brightness, 0, 1, BRIGHTNESS_MIN, BRIGHTNESS_MAX)));
}



void Segment7Display::clear() {
	if(decodeModeOn)
		setDecode(false);

	for(int i = 0; i <= DIGITS_AMOUNT; ++i)
		printByte(i, 0);
}



void Segment7Display::printByte(int digit, byte data) {
	if(digit >= 0 && digit < DIGITS_AMOUNT)
		sendPacket(preparePacket(digit + 1, data));
}



void Segment7Display::printChar(int digit, char symbol, boolean decimalPoint) {
	if(decodeModeOn)
		setDecode(false);

	printByte(digit, char2byte(symbol, decimalPoint));
}



void Segment7Display::printString(String str, byte align, boolean clearExcess, int offset, int maxLength) {
	if(decodeModeOn)
		setDecode(false);

	if(offset > DIGITS_AMOUNT)
		offset = DIGITS_AMOUNT;

	if(maxLength < 0)
		maxLength = DIGITS_AMOUNT - offset;
	else
		maxLength = min(DIGITS_AMOUNT - offset, maxLength);



	int i = 0;
	byte *bytes = new byte[maxLength];
	unsigned int bytesAmount = string2bytes(str, bytes, maxLength, align);

	switch(align) {
		case ALIGN_LEFT:
			if(clearExcess)
				for(; i < offset; ++i)
					printByte(DIGITS_AMOUNT - i - 1, 0);
			else
				i = offset;

			for(int j = 0; j < bytesAmount; ++j) {
				printByte(DIGITS_AMOUNT - i - 1, bytes[j]);
				++i;
			}

			for(; i < DIGITS_AMOUNT; ++i)
				printByte(DIGITS_AMOUNT - i - 1, 0);

			break;

		case ALIGN_RIGHT:
			if(clearExcess)
				for(; i < offset; ++i)
					printByte(i, 0);
			else
				i = offset;

			for(int j = 0; j < bytesAmount; ++j) {
				printByte(i, bytes[bytesAmount - j - 1]);
				++i;
			}

			for(; i < DIGITS_AMOUNT; ++i)
				printByte(i, 0);

			break;
	}

	delete bytes;
}



void Segment7Display::printFloat(float num, unsigned char precision, boolean E, byte align, boolean clearExcess, int offset, int maxLength) {
	if(offset > DIGITS_AMOUNT)
		offset = DIGITS_AMOUNT;

	if(maxLength < 0)
		maxLength = DIGITS_AMOUNT - offset;
	else
		maxLength = min(DIGITS_AMOUNT - offset, maxLength);

	char *str = new char[maxLength];

	if(E) {
		dtostre(num, str, precision, 0x04);
		for(int i = 0; i < maxLength - 1; ++i)
			if(str[i] == 'E') {
				if(str[++i] == '+') {
					for(; i < maxLength - 1; ++i)
						str[i] = str[i + 1];
					str[i] = 0;
				}
				break;
			}
	} else
		dtostrf(num, 1, precision, str);

	printString(str, align, clearExcess, offset, maxLength);

	delete str;
}



byte Segment7Display::char2byte(char symbol, boolean decimalPoint) {
	byte data = 0;
	if(symbol >= 0 && symbol < CHARS_AMOUNT)
		data = charTable[symbol];

	return data | decimalPoint << 7;
}



unsigned int Segment7Display::string2bytes(String str, byte *output, int maxLength, byte align) {
	int strLen = str.length(),
		strPos;
	byte temp;
	unsigned int length = 0;
	boolean pointTaken;

	switch(align) {
		case ALIGN_LEFT:
			strPos = 0;
			pointTaken = true;

			for(; strPos < strLen && length < maxLength; ++strPos)
				if(str[strPos] == '.') {
					if(pointTaken) {
						output[length] = 0x80;
						++length;
					} else {
						output[length - 1] |= 0x80;
						pointTaken = true;
					}
				} else {
					output[length] = char2byte(str[strPos]);
					pointTaken = false;
					++length;
				}

			break;

		case ALIGN_RIGHT:
			strPos = strLen - 1; 
			pointTaken = false;

			for(; strPos >= 0 && length < maxLength; --strPos)
				if(str[strPos] == '.') {
					if(pointTaken) {
						output[length] = 0x80;
						++length;
					} else {
						pointTaken = true;
					}
				} else {
					output[length] = char2byte(str[strPos], pointTaken);
					pointTaken = false;
					++length;
				}

			if(length < maxLength && pointTaken) {
				output[length] = 0x80;
				++length;
			}

			for(int j = 0, l = length / 2; j < l; ++j) {
				temp = output[j];
				output[j] = output[length - j - 1];
				output[length - j - 1] = temp;
			}

			break;
	}

	return length;
}



word Segment7Display::preparePacket(byte comand, byte data) {
	return (word)data | ((word)(comand & 0x0f) << 8);
}



void Segment7Display::sendPacket(word packet) {
	digitalWrite(pinStrobe, LOW);
	shiftOut(pinData, pinClock, MSBFIRST, packet >> 8);
	shiftOut(pinData, pinClock, MSBFIRST, packet);
	digitalWrite(pinStrobe, HIGH);
}



int Segment7Display::mapNormal(float value, int fromMin, int fromMax, int toMin, int toMax) {
	return (value - fromMin) / (fromMax - fromMin) * (toMax - toMin) + toMin + 0.4999;
}