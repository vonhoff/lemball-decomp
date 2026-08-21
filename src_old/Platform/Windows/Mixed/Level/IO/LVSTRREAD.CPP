#include "Visos/Generic/Memory.h"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/VSSTRM.H"

extern void UpdateNetworkFrameClockFromNetworkTimeThunk(unsigned int nTimeMs);

// FUNCTION: LEMBALL 0x00453070
void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStream(void* pObject)
{
	CNetworkMessage* pStream;
	unsigned int nTag;

	pStream = (CNetworkMessage*) pObject;
	nTag = pStream->ReadEffStreamU16BEValue();
	while (nTag != 0x2f) {
		switch (nTag) {
		case 0x2c: {
			unsigned int nStreamIndex = pStream->ReadEffStreamU8Value();
			char* pEntryStream = *(char**) (*(char**) ((char*) pObject + 0x30) + 0x10 + nStreamIndex * 4);
			pEntryStream += 0x138;
			if (((GameEffStream*) pEntryStream)->LoadEffStreamFromMemory(*(int*) ((char*) pObject + 0x20)) != 0) {
				*(int*) ((char*) pObject + 0x20) = *(int*) (pEntryStream + 0x20);
			}
			break;
		}
		case 0x2d:
			UpdateNetworkFrameClockFromNetworkTimeThunk(pStream->ReadEffStreamU32BEValue());
			break;
		}
		nTag = pStream->ReadEffStreamU16BEValue();
	}
}
