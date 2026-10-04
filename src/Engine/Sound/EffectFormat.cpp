#include "EffectFormat.h"

enum {
	LOW_WORD_MASK = 0xffff
};

// FUNCTION: LEMBALL 0x0047c210
unsigned short SwapBytes16(unsigned short p_value)
{
	unsigned short high = (unsigned short) (p_value >> 8);
	p_value = (unsigned short) (p_value << 8);
	return (unsigned short) (high + p_value);
}

// FUNCTION: LEMBALL 0x0047c230
unsigned int SwapBytes32(unsigned int p_value)
{
	unsigned int low = p_value & LOW_WORD_MASK;
	p_value >>= 16;
	low = SwapBytes16((unsigned short) low);
	low <<= 16;
	return SwapBytes16((unsigned short) p_value) + low;
}
