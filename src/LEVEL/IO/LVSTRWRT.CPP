#include "Visos/Generic/Memory.h"
#include "ENGINE/MEDIA/VSSTRM.H"

extern int g_nLevelFrameClockTimeMs;

// FUNCTION: LEMBALL 0x00453030
void LEMBALL_FASTCALL WriteNetworkLevelChunkDeltaStream(void* pObject)
{
	VsNetEffStreamBase* pStream = (VsNetEffStreamBase*) pObject;
	char* pOwnerStream;

	pStream->WriteEffStreamU16BE(0x2d);
	pStream->WriteEffStreamU32BE((unsigned int) g_nLevelFrameClockTimeMs);
	pOwnerStream = *(char**) ((char*) pObject + 0x2c) + 0x1c;
	((VsNetEffStreamBase*) pOwnerStream)->SaveEffStreamToMemoryRange(*(int*) ((char*) pObject + 0x1c), 0);
	*(int*) ((char*) pObject + 0x1c) += *(int*) (pOwnerStream + 0x1c) - *(int*) (pOwnerStream + 8);
	pStream->WriteEffStreamU16BE(0x2f);
}
