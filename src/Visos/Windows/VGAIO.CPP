#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

#include <conio.h>

#pragma intrinsic(_outpw)

// FUNCTION: LEMBALL 0x00457070
void LEMBALL_STDCALL WriteVgaSequencerMapMask(unsigned char nPlaneMask)
{
	_outpw(0x3c4, ((unsigned short) nPlaneMask << 8) | 2);
}
