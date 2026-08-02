#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

class HelperUploadStateMarkerView {
public:
	void PushHelperUploadStateMarker(int nMarker);
};

class LevelRenderHelperProviderInterface {
public:
	virtual void Slot0(void) = 0;
	virtual void Slot1(void) = 0;
	virtual HelperUploadStateMarkerView* GetUploadStateHelper(void) = 0;
};

struct LevelRenderHelperContextView {
	unsigned char m_abReserved00[0x0c];
	LevelRenderHelperProviderInterface* m_pProvider0C;
};

class LevelRenderStatePushEntry {
	unsigned char m_abReserved00[4];
	int m_nMarker04;

public:
	void Execute(LevelRenderHelperContextView* pContext);
};

class LevelRenderEntry;

class LevelRenderEntryOwnerInterface {
public:
	virtual void Dispatch(LevelRenderEntry* pEntry) = 0;
};

struct LevelRenderEntryContextView {
	unsigned char m_abReserved00[0x0c];
	unsigned char* m_pOwner0C;
};

class LevelRenderEntry {
public:
	void DispatchToOwner(LevelRenderEntryContextView* pContext);
};

// FUNCTION: LEMBALL 0x00439770
void LevelRenderStatePushEntry::Execute(LevelRenderHelperContextView* pContext)
{
	HelperUploadStateMarkerView* pHelper;

	pHelper = pContext->m_pProvider0C->GetUploadStateHelper();
	pHelper->PushHelperUploadStateMarker(m_nMarker04);
}

// FUNCTION: LEMBALL 0x004398e0
void LevelRenderEntry::DispatchToOwner(LevelRenderEntryContextView* pContext)
{
	((LevelRenderEntryOwnerInterface*) (pContext->m_pOwner0C + 0x98))->Dispatch(this);
}
