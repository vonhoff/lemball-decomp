#include "Visos/Generic/CVSMath.h"

// FUNCTION: LEMBALL 0x0045a9b0
unsigned int CVSMath::SqRoot(unsigned int uValue)
{
	unsigned int uLow;
	unsigned int uHigh;
	unsigned int uMid;

	if (uValue > 0x4000000) {
		if (uValue > 0x40000000) {
			uHigh = (uValue >> 0xf) + 1;
			uLow = 0x8000;
		}
		else if (uValue > 0x10000000) {
			uHigh = (uValue >> 0xe) + 1;
			uLow = 0x4000;
		}
		else {
			uHigh = (uValue >> 0xd) + 1;
			uLow = 0x2000;
		}
	}
	else if (uValue > 0x4000) {
		if (uValue > 0x1000000) {
			uHigh = (uValue >> 0xc) + 1;
			uLow = 0x1000;
		}
		else if (uValue > 0x100000) {
			uHigh = (uValue >> 10) + 1;
			uLow = 0x400;
		}
		else {
			uHigh = (uValue >> 7) + 1;
			uLow = 0x80;
		}
	}
	else if (uValue > 0x100) {
		uHigh = (uValue >> 4) + 1;
		uLow = 0x10;
	}
	else {
		uHigh = uValue + 1;
		uLow = 0;
	}

	if (uHigh == uLow) {
		return uLow;
	}

	while (uLow - uHigh != (unsigned int) -1) {
		uMid = (uLow + uHigh) >> 1;
		if (uMid * uMid <= uValue) {
			uLow = uMid;
		}
		else {
			uHigh = uMid;
		}
	}
	return uLow;
}
