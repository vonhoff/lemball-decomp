#include "ENGINE/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation in LINKSCF.CPP.

// FUNCTION: LEMBALL 0x00409ec0
void LEMBALL_FASTCALL RemoveInvsTrackedEntitiesOutsideBounds(void* pObject)
{
	char* pObjectBytes = (char*) pObject;
	int iEntity = 0;
	while (iEntity < *(int*) (pObjectBytes + 0x254)) {
		char* pEntity = *(char**) (pObjectBytes + 0x258 + iEntity * 4);
		int nX = *(int*) (pEntity + 0x9c) >> 12;
		int nY = *(int*) (pEntity + 0xa0) >> 12;
		if (nX < *(short*) (pObjectBytes + 0x138) - 8 || *(short*) (pObjectBytes + 0x13e) + 7 < nX ||
			nY < *(short*) (pObjectBytes + 0x13a) - 8 || *(short*) (pObjectBytes + 0x140) + 7 < nY) {
			int iMove;
			*(unsigned short*) (pEntity + 0x120) = 0xffff;
			for (iMove = iEntity + 1; iMove < *(int*) (pObjectBytes + 0x254); ++iMove) {
				*(void**) (pObjectBytes + 0x258 + (iMove - 1) * 4) = *(void**) (pObjectBytes + 0x258 + iMove * 4);
			}
			--*(int*) (pObjectBytes + 0x254);
			continue;
		}
		++iEntity;
	}
}

// FUNCTION: LEMBALL 0x00409f70
void LEMBALL_FASTCALL AddInvsTrackedEntity(void* pObject, void* pUnused, void* pEntity)
{
	char* pObjectBytes = (char*) pObject;
	int cEntities = *(int*) (pObjectBytes + 0x254);
	(void) pUnused;
	if (cEntities < 0x18) {
		*(void**) (pObjectBytes + 0x258 + cEntities * 4) = pEntity;
		++*(int*) (pObjectBytes + 0x254);
		*(unsigned short*) ((char*) pEntity + 0x120) = GetManagedEntitySlotIdThunk((int) (unsigned long) pObject);
	}
}
