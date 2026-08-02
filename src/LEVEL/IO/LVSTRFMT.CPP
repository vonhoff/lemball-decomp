#include "ENGINE/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x00407e40
void* LEMBALL_FASTCALL DeleteFormattedOutputStream(void* pState, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	RestoreStreamFormatSubobjectVtable((VsInitStreamFormatTargetState*) pState);
	ConstructStreamFormatState((VsInitStreamFormatTargetState*) pState);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock((char*) pState - 0x12c);
	}
	return (char*) pState - 0x12c;
}
