extern void* g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8];
extern void* g_LEVEL_CompositePointRectSinkEntryVtable[8];
extern void* g_LEVEL_FramedScreenRenderChildEntryVtableStorage[8];

struct LevelSelectionRenderChildEntry {
	void* m_pVtable00;
	short m_nLeft04;
	short m_nTop06;
	short m_nRight08;
	short m_nBottom0a;
	short m_nSourceLeft0c;
	short m_nSourceTop0e;
	char m_abReserved10[0x10];
	short m_nSourceRight20;
	short m_nSourceBottom22;

	LevelSelectionRenderChildEntry* Initialize(void);
};

// FUNCTION: LEMBALL 0x0044a970
LevelSelectionRenderChildEntry* LevelSelectionRenderChildEntry::Initialize(void)
{
	int* pEntry;

	pEntry = (int*) this;
	*(void**) pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	*(unsigned short*) ((char*) pEntry + 6) = 0;
	*(unsigned short*) (pEntry + 1) = 0;
	*(void**) pEntry = g_LEVEL_CompositePointRectSinkEntryVtable;
	*(unsigned short*) ((char*) pEntry + 10) = 0;
	*(unsigned short*) (pEntry + 2) = 0;
	*(void**) pEntry = g_LEVEL_FramedScreenRenderChildEntryVtableStorage;
	*(unsigned short*) ((char*) pEntry + 0xe) = 0;
	*(unsigned short*) (pEntry + 3) = 0;
	*(unsigned short*) ((char*) pEntry + 0x22) = 0;
	*(unsigned short*) (pEntry + 8) = 0;
	return this;
}
