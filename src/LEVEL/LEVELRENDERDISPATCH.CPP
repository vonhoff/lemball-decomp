#include "ENGINE/CORE/COMMON.H"

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

// FUNCTION: LEMBALL 0x004398e0
void LevelRenderEntry::DispatchToOwner(LevelRenderEntryContextView* pContext)
{
	((LevelRenderEntryOwnerInterface*) (pContext->m_pOwner0C + 0x98))->Dispatch(this);
}
