#include "AI/CHand.h"
#include "Visos/Generic/Memory.h"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Mixed/Level/CHUNKOBJVT.H"

extern void LEMBALL_FASTCALL DestroyCTrampoline(void* pObject);
extern void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject);

// Split from CHUNKOBJVT.CPP to preserve MSVC 4.20 code generation.

// FUNCTION: LEMBALL 0x00428800
void* LEMBALL_FASTCALL DeleteCaptureChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	char* pCursor;
	char* pAllocation;
	unsigned int cObjects;

	(void) pUnused;
	if ((fDelete & 2) != 0) {
		pAllocation = (char*) pObject - sizeof(int);
		cObjects = *(unsigned int*) pAllocation;
		pCursor = (char*) pObject + cObjects * 0x144;
		while ((int) --cObjects >= 0) {
			pCursor -= 0x144;
			((CHand*) pCursor)->~CHand();
		}
		FreeVSMemBlock(pAllocation);
		return pObject;
	}
	((CHand*) pObject)->~CHand();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00429ed0
void* LEMBALL_FASTCALL DeleteLasrChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	char* pAllocation;
	char* pCursor;

	(void) pUnused;
	if ((fDelete & 2) != 0) {
		pAllocation = (char*) pObject - sizeof(int);
		fDelete = *(unsigned int*) pAllocation;
		pCursor = (char*) pObject + fDelete * 0x148;
		while ((int) --fDelete >= 0) {
			pCursor -= 0x148;
			destroy_lasr_chunk_object_vtable_thunk(pCursor);
		}
		FreeVSMemBlock(pAllocation);
		return pObject;
	}
	destroy_lasr_chunk_object_vtable_thunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x0042ba40
void* LEMBALL_FASTCALL DeleteTramChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	char* pAllocation;
	char* pCursor;

	(void) pUnused;
	if ((fDelete & 2) != 0) {
		pAllocation = (char*) pObject - sizeof(int);
		fDelete = *(unsigned int*) pAllocation;
		pCursor = (char*) pObject + fDelete * 0x144;
		while ((int) --fDelete >= 0) {
			pCursor -= 0x144;
			DestroyCTrampoline(pCursor);
		}
		FreeVSMemBlock(pAllocation);
		return pObject;
	}
	DestroyCTrampoline(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}
