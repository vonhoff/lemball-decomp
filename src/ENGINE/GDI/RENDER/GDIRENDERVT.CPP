#include "Visos/Generic/Memory.h"
#include "ENGINE/GDI/VSGDI.H"

extern void LEMBALL_FASTCALL DispatchLevelScreenRenderStatePop(void* pEntry, int nUnused, void* pContext);

// LINKERILT: LEMBALL 0x00401073
void LEMBALL_FASTCALL ExecuteStatusRenderPopThunk(void* pEntry, int nUnused, void* pContext)
{
	DispatchLevelScreenRenderStatePop(pEntry, nUnused, pContext);
}

// LINKERILT: LEMBALL 0x00401334
void LEMBALL_FASTCALL ExecuteStatusPointPushThunk(void* pEntry, int nUnused, void* pContext)
{
	(void) pEntry;
	(void) nUnused;
	(void) pContext;
}

// LINKERILT: LEMBALL 0x0040189d
void* LEMBALL_FASTCALL DeleteStatusPointSinkEntryThunk(void* pEntry, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pEntry);
	}
	return pEntry;
}

// LINKERILT: LEMBALL 0x004021b2
void* LEMBALL_FASTCALL DeleteStatusRenderEntryThunk(void* pEntry, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pEntry);
	}
	return pEntry;
}

// LINKERILT: LEMBALL 0x004022c5
void* LEMBALL_FASTCALL DeleteStatusPointSinkVariantThunk(void* pEntry, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pEntry);
	}
	return pEntry;
}

// LINKERILT: LEMBALL 0x004023fb
void LEMBALL_FASTCALL SetStatusRenderEntryFieldThunk(void* pEntry, int nUnused, int nValue)
{
	(void) nUnused;
	*(int*) ((char*) pEntry + 4) = nValue;
}

// LINKERILT: LEMBALL 0x00402b35
void LEMBALL_FASTCALL DispatchStatusPointSinkOwnerThunk(void* pEntry, int nUnused, void* pContext)
{
	typedef void(LEMBALL_FASTCALL * DispatchProc)(void*, int, void*);
	void* pOwner = *(void**) ((char*) pContext + 0x0c);
	(void) nUnused;
	((DispatchProc) (*(void***) pOwner)[38])(pOwner, 0, pEntry);
}

// LINKERILT: LEMBALL 0x0040309e
void LEMBALL_FASTCALL AppendStatusPointSinkToQueueThunk(void* pEntry, int nUnused, void* pQueue)
{
	(void) nUnused;
	((VsGdiHelperDispatchQueue*) pQueue)->AppendPointerQueueEntry(pEntry);
}

// LINKERILT: LEMBALL 0x00403512
void LEMBALL_FASTCALL QueueStatusRenderPopThunk(void* pEntry, int nUnused, void* pQueue)
{
	(void) nUnused;
	((VsGdiHelperDispatchQueue*) pQueue)->AppendPointerQueueEntry(pEntry);
}

// LINKERILT: LEMBALL 0x00403670
void LEMBALL_FASTCALL QueueStatusPointPushThunk(void* pEntry, int nUnused, void* pQueue)
{
	(void) nUnused;
	((VsGdiHelperDispatchQueue*) pQueue)->AppendPointerQueueEntry(pEntry);
}

// LINKERILT: LEMBALL 0x004036a7
int LEMBALL_FASTCALL ReturnZeroStatusRenderThunk(void* pEntry)
{
	(void) pEntry;
	return 0;
}
