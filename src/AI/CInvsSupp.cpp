#include "ENGINE/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct InvsChunkManagerActionView {
	void DispatchContactsForEntity(const int* pPosition, void* pEntity);
};

struct InvsChunkObjectActionView {
	void AddInvsTrackedEntity(void* pEntity);
	void TryTriggerContact(const int* pPosition, void* pEntity);
};

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
void InvsChunkObjectActionView::AddInvsTrackedEntity(void* pEntity)
{
	char* pObjectBytes = (char*) this;
	int cEntities = *(int*) (pObjectBytes + 0x254);
	if (cEntities < 0x18) {
		*(void**) (pObjectBytes + 0x258 + cEntities * 4) = pEntity;
		++*(int*) (pObjectBytes + 0x254);
		*(unsigned short*) ((char*) pEntity + 0x120) = GetManagedEntitySlotIdThunk((int) (unsigned long) this);
	}
}

// FUNCTION: LEMBALL 0x00409fa0
void InvsChunkObjectActionView::TryTriggerContact(const int* pPosition, void* pEntity)
{
	char* pObjectBytes = (char*) this;
	char* pEntityBytes = (char*) pEntity;
	int nX;
	int nY;

	if (*(int*) (pObjectBytes + 0x148) != 0 || *(int*) (pObjectBytes + 0x12c) != 0x18 ||
		GetManagedEntitySlotIdThunk((int) (unsigned long) this) == *(unsigned short*) (pEntityBytes + 0x120)) {
		return;
	}
	nX = pPosition[0] >> 12;
	nY = pPosition[1] >> 12;
	if (nX < *(short*) (pObjectBytes + 0x138) - 8 || *(short*) (pObjectBytes + 0x13e) + 7 < nX ||
		nY < *(short*) (pObjectBytes + 0x13a) - 8 || *(short*) (pObjectBytes + 0x140) + 7 < nY) {
		return;
	}
	*(void**) (pObjectBytes + 0x5c) = pEntity;
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
}

// FUNCTION: LEMBALL 0x0040a370
void InvsChunkManagerActionView::DispatchContactsForEntity(const int* pPosition, void* pEntity)
{
	char* pManagerBytes;
	int iObject;

	pManagerBytes = (char*) this;
	iObject = 0;
	if (iObject < *(int*) ((char*) this + 0x34)) {
		int nObjectOffset;

		nObjectOffset = 0;
		do {
			InvsChunkObjectActionView* pObject;

			pObject = (InvsChunkObjectActionView*) (*(char**) (pManagerBytes + 0x3c) + nObjectOffset);
			pObject->TryTriggerContact(pPosition, pEntity);
			nObjectOffset += 0x2b8;
			++iObject;
		} while (iObject < *(int*) (pManagerBytes + 0x34));
	}
}
