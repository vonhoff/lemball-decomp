#include "ENGINE/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"

// LINKERILT: LEMBALL 0x00401d2a
void* LEMBALL_FASTCALL DeleteStreamFormatTargetState(void* pState, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	ConstructStreamFormatState((VsInitStreamFormatTargetState*) pState);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pState);
	}
	return pState;
}
