#ifndef LEMBALL_DRAWTEXT_H
#define LEMBALL_DRAWTEXT_H

#include "GAME.H"
#include "ENGINE/CORE/VSMEM.H"

class LevelVariantRenderManagerView {
public:
    void *Construct(int nOwner, int cResourceIds,
                    int cPackagedRectEntries, int cActionButtonEntries,
                    int nUnknown58, int fDoubleBuffered);
    void LoadVariantRenderResource(int nResourceId);
    void ReleaseVariantRenderResource(int nResourceId);
    int GetVariantRenderFrameCount(int nResourceId);
    short *GetVariantFrameExtent(
        short *pExtent, int nResourceId, int iFrame);
    void ReleaseLevelScreenVariantStateWrapperBody(int nResourceId);
    void ReleaseLevelScreenVariantStateWrapper(int nResourceId);
    void ReleaseLevelScreenVariantStateWrapperRangeBody(
        int nFirstResourceId, int nLastResourceId);
    void ReleaseLevelScreenVariantStateWrapperRange(
        int nFirstResourceId, int nLastResourceId);
    void EmitVariantRenderResourceEntry(
        short *pRect, const short *pPoint, int nResourceId,
        int nFlags, void *pFrameSelector, int nValue);
    void ResetOrFlipVariantRenderBuffers(void);

    char m_abReserved00[0x24];
    void **m_ppResourceObjects24;
    short *m_pResourceIds28;
    int m_nFreeResourceId2C;
    int m_nReserved30;
    int m_cLoadedResources34;
};

inline void LoadVariantRenderResource(void *pManager, int nResourceId) {
    ((LevelVariantRenderManagerView *)pManager)
        ->LoadVariantRenderResource(nResourceId);
}

class LevelQueuedRenderPointSink {
public:
    virtual ~LevelQueuedRenderPointSink() {}
    virtual void AppendToQueue(void *pQueue) = 0;
    virtual void Process(void *pContext) = 0;
};

// SYNTHETIC: LEMBALL 0x00467b40
// LevelPackagedRectQueueEntry::`scalar deleting destructor'
// VTABLE: LEMBALL 0x00499628
// SIZE 0x18
class LevelPackagedRectQueueEntry : public LevelQueuedRenderPointSink {
public:
    LevelPackagedRectQueueEntry();
    virtual ~LevelPackagedRectQueueEntry();
    virtual void AppendToQueue(void *pQueue);
    virtual void Process(void *pContext);

    short m_x04;
    short m_y06;
    void *m_pOwner08;
    int m_nFlags0C;
    int m_nValue10;
    int m_nReserved14;
};

// SYNTHETIC: LEMBALL 0x00469b80
// LevelScreenDrawEntry::`scalar deleting destructor'
// VTABLE: LEMBALL 0x00499ae8
// SIZE 0x44
class LevelScreenDrawEntry : public LevelQueuedRenderPointSink {
public:
    LevelScreenDrawEntry();
    virtual ~LevelScreenDrawEntry();
    virtual void AppendToQueue(void *pQueue);
    virtual void Process(void *pRenderContext);
    virtual void InitializeFromPointAndDynamicText(const short *pPoint, void *pOwner,
                            GameDynamicCString Text,
                            int nDrawFlags, int nDrawValue);
    virtual void InitializeFromPositionAndDynamicText(int x, int y, void *pOwner,
                            GameDynamicCString Text,
                            int nDrawFlags, int nDrawValue);
    virtual void InitializeFromPointAndText(const short *pPoint, void *pOwner, void *pText,
                            int nDrawFlags, int nDrawValue);
    virtual void InitializeFromPositionAndText(int x, int y, void *pOwner, void *pText,
                            int nDrawFlags, int nDrawValue);

    void AdvanceCursor();

    void *m_pOwner04;
    void *m_pText08;
    short m_xCurrent0C;
    short m_yCurrent0E;
    int m_nDrawFlags10;
    int m_nDrawValue14;
    short m_nReserved18;
    short m_nReserved1A;
    short m_xStart1C;
    short m_yStart1E;
    int m_nQueueState20;
    short m_nReserved24;
    short m_nReserved26;
    void *m_pGlyph28;
    LevelPackagedRectQueueEntry m_DrawSink2C;
};

// SYNTHETIC: LEMBALL 0x0046a220
// LevelResourceScreenDrawEntry::`scalar deleting destructor'
// VTABLE: LEMBALL 0x00499ce8
// SIZE 0x4c
class LevelResourceScreenDrawEntry : public LevelScreenDrawEntry {
public:
    LevelResourceScreenDrawEntry(int cchText) {
        m_cchOwnedText48 = cchText;
        m_pszOwnedText44 = (char *)AllocateVSMemBlock(cchText + 1);
    }
    virtual ~LevelResourceScreenDrawEntry() {
        FreeVSMemBlock(m_pszOwnedText44);
    }
    virtual void InitializeFromPointAndDynamicText(const short *pPoint, void *pOwner,
                            GameDynamicCString Text,
                            int nDrawFlags, int nDrawValue);
    virtual void InitializeFromPositionAndDynamicText(int x, int y, void *pOwner,
                            GameDynamicCString Text,
                            int nDrawFlags, int nDrawValue);
    virtual void InitializeFromPointAndText(const short *pPoint, void *pOwner, void *pText,
                            int nDrawFlags, int nDrawValue);
    virtual void InitializeFromPositionAndText(int x, int y, void *pOwner, void *pText,
                            int nDrawFlags, int nDrawValue);

    char *m_pszOwnedText44;
    int m_cchOwnedText48;
};

class MappedListResourceTable {
public:
    MappedListResourceTable(int cResourceIds, int cSlots,
                            int cDrawEntries, int cchOwnedText);
    void LoadListResourceIntoSlot(int nResourceId);
    void ReleaseListResourceSlot(int nResourceId);
    void *GetMappedResource(int nResourceId);
    void SubmitRenderEntry(void *pRenderQueue, const short *pPoint,
                           const short *pRelativePoint, int nResourceId,
                           void *pText, unsigned int nDrawFlags,
                           int nDrawValue);
    void Destroy(void);
    void ResetDrawEntryPool(void) { m_nCurrentSlot18 = 0; }

    void **m_apResources00;
    short *m_aiResourceSlots04;
    int m_cSlots08;
    int m_cResourceIds0C;
    int m_nReserved10;
    int m_cDrawEntries14;
    int m_nCurrentSlot18;
    int m_nReserved1C;
    LevelScreenDrawEntry **m_apDrawEntries20;
};

#endif
