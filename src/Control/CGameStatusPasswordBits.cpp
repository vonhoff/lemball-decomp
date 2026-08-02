#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

int LEMBALL_STDCALL ComputePowerOfTwoThunk(int nExponent);

// FUNCTION: LEMBALL 0x00406be0
unsigned int LEMBALL_STDCALL ExtractLevelPasswordBitField(unsigned int uValue, unsigned char nShift, int nWidth)
{
	unsigned int uMask;

	uMask = ComputePowerOfTwoThunk(nWidth) - 1;
	uValue >>= nShift;
	return uMask & uValue;
}
