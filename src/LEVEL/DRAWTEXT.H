#ifndef LEMBALL_DRAWTEXT_H
#define LEMBALL_DRAWTEXT_H

#include "Visos/Generic/Memory.h"
#include "GAME.H"

class LevelVariantRenderManagerView {
public:
	void* Construct(int nOwner,
					int cResourceIds,
					int cPackagedRectEntries,
					int cActionButtonEntries,
					int nUnknown58,
					int fDoubleBuffered);
	void LoadVariantRenderResource(int nResourceId);
	void ReleaseVariantRenderResource(int nResourceId);
	int GetVariantRenderFrameCount(int nResourceId);
	short* GetVariantFrameExtent(short* pExtent, int nResourceId, int iFrame);
	void ReleaseLevelScreenVariantStateWrapperBody(int nResourceId);
	void ReleaseLevelScreenVariantStateWrapper(int nResourceId);
	void ReleaseLevelScreenVariantStateWrapperRangeBody(int nFirstResourceId, int nLastResourceId);
	void ReleaseLevelScreenVariantStateWrapperRange(int nFirstResourceId, int nLastResourceId);
	void EmitVariantRenderResourceEntry(short* pRect,
										const short* pPoint,
										int nResourceId,
										int nFlags,
										void* pFrameSelector,
										int nValue);
	void LoadLevelScreenVariantState(int nResourceId, int nSelectorMode);
	void ResetOrFlipVariantRenderBuffers(void);

	char m_abReserved00[0x24];
	void** m_ppResourceObjects24;
	short* m_pResourceIds28;
	int m_nFreeResourceId2C;
	int m_nReserved30;
	int m_cLoadedResources34;
};

inline void LoadVariantRenderResource(void* pManager, int nResourceId)
{
	((LevelVariantRenderManagerView*) pManager)->LoadVariantRenderResource(nResourceId);
}

// SIZE 0x04
class LevelQueuedRenderPointSink {
public:
	~LevelQueuedRenderPointSink() {}
	virtual void* Delete(unsigned int fDelete) = 0;
	virtual void AppendToQueue(void* pQueue) = 0;
	virtual void Process(void* pContext) = 0;
};

// VTABLE: LEMBALL 0x00499628
// SIZE 0x18
class LevelPackagedRectQueueEntry : public LevelQueuedRenderPointSink {
public:
	LevelPackagedRectQueueEntry();
	~LevelPackagedRectQueueEntry() {}
	virtual void* Delete(unsigned int fDelete); // vtable+0x00
	virtual void AppendToQueue(void* pQueue); // vtable+0x04
	virtual void Process(void* pContext); // vtable+0x08
	void ResetVtableAfterDestruction();

	short m_x04; // 0x04
	short m_y06; // 0x06
	void* m_pOwner08; // 0x08
	int m_nFlags0C; // 0x0c
	int m_nValue10; // 0x10
	int m_nReserved14; // 0x14
};

// VTABLE: LEMBALL 0x00499ae8
// SIZE 0x44
class LevelScreenDrawEntry : public LevelQueuedRenderPointSink {
public:
	LevelScreenDrawEntry();
	~LevelScreenDrawEntry() {}
	virtual void* Delete(unsigned int fDelete); // vtable+0x00
	virtual void AppendToQueue(void* pQueue); // vtable+0x04
	virtual void Process(void* pRenderContext); // vtable+0x08
	void ResetVtablesAfterDestruction();
	virtual void InitializeFromPointAndDynamicText(const short* pPoint,
												   void* pOwner,
												   GameDynamicCString Text,
												   int nDrawFlags,
												   int nDrawValue); // vtable+0x0c
	virtual void InitializeFromPositionAndDynamicText(int x,
													  int y,
													  void* pOwner,
													  GameDynamicCString Text,
													  int nDrawFlags,
													  int nDrawValue); // vtable+0x10
	virtual void InitializeFromPointAndText(const short* pPoint,
											void* pOwner,
											void* pText,
											int nDrawFlags,
											int nDrawValue); // vtable+0x14
	virtual void InitializeFromPositionAndText(int x, int y, void* pOwner, void* pText, int nDrawFlags, int nDrawValue); // vtable+0x18

	void AdvanceCursor();

	void* m_pOwner04; // 0x04
	void* m_pText08; // 0x08
	short m_xCurrent0C; // 0x0c
	short m_yCurrent0E; // 0x0e
	int m_nDrawFlags10; // 0x10
	int m_nDrawValue14; // 0x14
	short m_nReserved18; // 0x18
	short m_nReserved1A; // 0x1a
	short m_xStart1C; // 0x1c
	short m_yStart1E; // 0x1e
	int m_nQueueState20; // 0x20
	short m_nReserved24; // 0x24
	short m_nReserved26; // 0x26
	void* m_pGlyph28; // 0x28
	LevelPackagedRectQueueEntry m_DrawSink2C; // 0x2c
};

// VTABLE: LEMBALL 0x00499ce8
// SIZE 0x4c
class LevelResourceScreenDrawEntry : public LevelScreenDrawEntry {
public:
	LevelResourceScreenDrawEntry(int cchText)
	{
		m_cchOwnedText48 = cchText;
		m_pszOwnedText44 = (char*) AllocateVSMemBlock(cchText + 1);
	}
	~LevelResourceScreenDrawEntry() { FreeVSMemBlock(m_pszOwnedText44); }
	virtual void* Delete(unsigned int fDelete); // vtable+0x00
	virtual void InitializeFromPointAndDynamicText(const short* pPoint,
												   void* pOwner,
												   GameDynamicCString Text,
												   int nDrawFlags,
												   int nDrawValue); // vtable+0x0c
	virtual void InitializeFromPositionAndDynamicText(int x,
													  int y,
													  void* pOwner,
													  GameDynamicCString Text,
													  int nDrawFlags,
													  int nDrawValue); // vtable+0x10
	virtual void InitializeFromPointAndText(const short* pPoint,
											void* pOwner,
											void* pText,
											int nDrawFlags,
											int nDrawValue); // vtable+0x14
	virtual void InitializeFromPositionAndText(int x, int y, void* pOwner, void* pText, int nDrawFlags, int nDrawValue); // vtable+0x18

	char* m_pszOwnedText44; // 0x44
	int m_cchOwnedText48; // 0x48
};

// SIZE 0x24
class MappedListResourceTable {
public:
	MappedListResourceTable(int cResourceIds, int cSlots, int cDrawEntries, int cchOwnedText);
	void LoadListResourceIntoSlot(int nResourceId);
	void ReleaseListResourceSlot(int nResourceId);
	void* GetMappedResource(int nResourceId);
	void SubmitRenderEntry(void* pRenderQueue,
						   const short* pPoint,
						   const short* pRelativePoint,
						   int nResourceId,
						   void* pText,
						   unsigned int nDrawFlags,
						   int nDrawValue);
	void Destroy(void);
	void ResetDrawEntryPool(void) { m_nCurrentSlot18 = 0; }

	void** m_apResources00; // 0x00
	short* m_aiResourceSlots04; // 0x04
	int m_cSlots08; // 0x08
	int m_cResourceIds0C; // 0x0c
	int m_nReserved10; // 0x10
	int m_cDrawEntries14; // 0x14
	int m_nCurrentSlot18; // 0x18
	int m_nReserved1C; // 0x1c
	LevelScreenDrawEntry** m_apDrawEntries20; // 0x20
};

#endif
