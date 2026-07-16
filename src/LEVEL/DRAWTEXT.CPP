#include "LEVEL/DRAWTEXT.H"

#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>

extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);

class LevelGlyphListView {
public:
    virtual void *GetGlyphListResource(void) = 0;
    virtual void *FindGlyph(unsigned int ch) = 0;
};

class LevelDrawDispatchView {
public:
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void DrawGlyph(LevelPackagedRectQueueEntry *pEntry,
                           void *pGlyph) = 0;
};

struct LevelScreenDrawOwnerView {
    void **m_pVtable00;
    int m_nReserved04;
    int m_cQueuedEntries08;
    int m_nReserved0C;
    int m_fImmediateQueue10;
    char m_abReserved14[0x10];
    int m_nQueueState24;
    char m_abReserved28[8];
    unsigned int m_nFontResourceId30;
    char m_abReserved34[0x44];
    LevelGlyphListView *m_pGlyphList78;
    void *m_pFallbackGlyph7C;

    void *LookupGlyph(unsigned int ch);
};

struct LevelDrawContextView {
    char m_abReserved00[0x0c];
    int m_nDispatchOffset0C;
};

static const char g_szMissingGlyphPrefix[] = "Letter '";
static const char g_szMissingGlyphText[] = "' not found in font ";
static const char g_szMissingGlyphNewline[] = "\n";

// FUNCTION: LEMBALL 0x0045BAD0
VsInitFormattedOutputStream *AppendUIntToStreamAsInt(VsInitFormattedOutputStream *pStream, unsigned int nValue) {
    pStream->AppendIntToStream(nValue);
    return pStream;
}

// FUNCTION: LEMBALL 0x0045DB20
void *LevelScreenDrawOwnerView::LookupGlyph(unsigned int ch) {
    return m_pGlyphList78->FindGlyph(ch);
}

LevelQueuedRenderPointSink::~LevelQueuedRenderPointSink() {
}

// FUNCTION: LEMBALL 0x00467AC0
LevelPackagedRectQueueEntry::LevelPackagedRectQueueEntry() {
    m_x04 = 0;
    m_y06 = 0;
}

// FUNCTION: LEMBALL 0x00467BA0
LevelPackagedRectQueueEntry::~LevelPackagedRectQueueEntry() {
}

// FUNCTION: LEMBALL 0x00467AE0
void LevelPackagedRectQueueEntry::AppendToQueue(void *pQueue) {
    int *pOwner;

    pOwner = (int *)m_pOwner08;
    if (pOwner[4] == 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int))(*(void ***)pOwner)[7])(
            pOwner, 0);
    } else {
        pOwner[9] = 0;
    }
    ++pOwner[2];
    AppendPointerQueueEntry(pQueue, this);
}

// FUNCTION: LEMBALL 0x00467B10
void LevelPackagedRectQueueEntry::Process(void *pContext) {
    int *pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void *pDispatchObject;

    pOwner = (int *)m_pOwner08;
    pOwnerOffsets = *(int *)(*(int *)((char *)pContext + 0x0c) + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatchObject = (void *)(unsigned long)(
        pDispatchBase + 0x40 + *(int *)((char *)pContext + 0x0c));
    ((void (LEMBALL_FASTCALL *)(void *, int, void *, void *))
        (*(void ***)pDispatchObject)[9])(
            pDispatchObject, 0, this, pOwner);
    --pOwner[2];
}

// FUNCTION: LEMBALL 0x00469A00
LevelScreenDrawEntry::LevelScreenDrawEntry() {
    m_xCurrent0C = 0;
    m_yCurrent0E = 0;
    m_nReserved18 = 0;
    m_nReserved1A = 0;
    m_xStart1C = 0;
    m_yStart1E = 0;
    m_nReserved24 = 0;
    m_nReserved26 = 0;
}

// FUNCTION: LEMBALL 0x00469BF0
LevelScreenDrawEntry::~LevelScreenDrawEntry() {
}

// FUNCTION: LEMBALL 0x00469B40
void LevelScreenDrawEntry::AppendToQueue(void *pQueue) {
    m_nQueueState20 = 0;
    AppendPointerQueueEntry(pQueue, this);
}

// FUNCTION: LEMBALL 0x004749C0
void LevelScreenDrawEntry::AdvanceCursor() {
    short cx;
    short cy;

    if (m_nQueueState20 == 0) {
        cx = *(short *)((char *)m_pGlyph28 + 0x4c) + 1;
        cy = *(short *)((char *)m_pGlyph28 + 0x4e) + 1;
    } else {
        cx = m_nReserved24;
        cy = m_nReserved26;
    }
    if ((m_nDrawFlags10 & 0x200) != 0) {
        cx = (short)(cx + m_nReserved18);
        cy = (short)(cy + m_nReserved1A);
    }
    if ((m_nDrawFlags10 & 0x40) != 0) {
        m_xCurrent0C = (short)(m_xCurrent0C - cx);
    } else if ((m_nDrawFlags10 & 0x20) != 0) {
        m_xCurrent0C = (short)(m_xCurrent0C + cx);
    }
    if ((m_nDrawFlags10 & 0x80) != 0) {
        m_yCurrent0E = (short)(m_yCurrent0E - cy);
    } else if ((m_nDrawFlags10 & 0x100) != 0) {
        m_yCurrent0E = (short)(m_yCurrent0E + cy);
    }
}

// FUNCTION: LEMBALL 0x00474A20
void LevelScreenDrawEntry::Process(void *pRenderContext) {
    LevelScreenDrawOwnerView *pOwner;
    const unsigned char *pch;
    LevelDrawContextView *pContext;
    int nDispatchBase;
    LevelDrawDispatchView *pDispatch;

    pOwner = (LevelScreenDrawOwnerView *)m_pOwner04;
    if (pOwner->m_fImmediateQueue10 != 0) {
        pOwner->m_nQueueState24 = 0;
    } else {
        ((void (LEMBALL_FASTCALL *)(void *))pOwner->m_pVtable00[7])(pOwner);
    }
    ++pOwner->m_cQueuedEntries08;
    m_xCurrent0C = m_xStart1C;
    m_DrawSink2C.m_nFlags0C = m_nDrawFlags10;
    m_yCurrent0E = m_yStart1E;
    m_DrawSink2C.m_nValue10 = m_nDrawValue14;

    pch = (const unsigned char *)m_pText08;
    while (*pch != 0) {
        m_pGlyph28 = ((LevelScreenDrawOwnerView *)m_pOwner04)->LookupGlyph(*pch);
        if (m_pGlyph28 == 0) {
            m_pGlyph28 = ((LevelScreenDrawOwnerView *)m_pOwner04)->LookupGlyph(0x49);
            if (m_pGlyph28 == 0) {
                m_pGlyph28 =
                    ((LevelScreenDrawOwnerView *)m_pOwner04)->m_pFallbackGlyph7C;
            }
            if (*pch != ' ' || m_pGlyph28 == 0) {
                AppendUIntToStreamAsInt(
                    g_pStatusOutputStream
                        ->AppendCStringToStream(g_szMissingGlyphPrefix)
                        ->AppendCharToStreamVariant((char)*pch)
                        ->AppendCStringToStream(g_szMissingGlyphText),
                    ((LevelScreenDrawOwnerView *)m_pOwner04)->m_nFontResourceId30)
                    ->AppendCStringToStream(g_szMissingGlyphNewline);
            }
            AdvanceCursor();
        } else {
            if ((m_nDrawFlags10 & 0xc0) != 0) {
                AdvanceCursor();
            }
            m_DrawSink2C.m_x04 = m_xCurrent0C;
            m_DrawSink2C.m_y06 = m_yCurrent0E;
            pContext = (LevelDrawContextView *)pRenderContext;
            nDispatchBase = *(int *)(
                *(int *)(pContext->m_nDispatchOffset0C + 0x40) + 4);
            pDispatch = (LevelDrawDispatchView *)(unsigned long)(
                nDispatchBase + pContext->m_nDispatchOffset0C + 0x40);
            pDispatch->DrawGlyph(&m_DrawSink2C, m_pGlyph28);
            if ((m_nDrawFlags10 & 0xc0) == 0) {
                AdvanceCursor();
            }
        }
        ++pch;
    }
    --((LevelScreenDrawOwnerView *)m_pOwner04)->m_cQueuedEntries08;
}

// FUNCTION: LEMBALL 0x00469B00
void LevelScreenDrawEntry::InitializeFromPointAndDynamicText(const short *pPoint, void *pOwner, GameDynamicCString Text, int nDrawFlags, int nDrawValue) {
    m_xStart1C = pPoint[0];
    m_yStart1E = pPoint[1];
    m_pOwner04 = pOwner;
    m_nDrawFlags10 = nDrawFlags;
    m_nDrawValue14 = nDrawValue;
    m_pText08 = Text.m_pszText;
    DestroyDynamicCString(&Text);
}

// FUNCTION: LEMBALL 0x00469AC0
void LevelScreenDrawEntry::InitializeFromPositionAndDynamicText(short x, short y, void *pOwner, GameDynamicCString Text, int nDrawFlags, int nDrawValue) {
    m_xStart1C = x;
    m_yStart1E = y;
    m_pOwner04 = pOwner;
    m_nDrawFlags10 = nDrawFlags;
    m_nDrawValue14 = nDrawValue;
    m_pText08 = Text.m_pszText;
    DestroyDynamicCString(&Text);
}

// FUNCTION: LEMBALL 0x00469A80
void LevelScreenDrawEntry::InitializeFromPointAndText(const short *pPoint, void *pOwner, void *pText, int nDrawFlags, int nDrawValue) {
    m_xStart1C = pPoint[0];
    m_yStart1E = pPoint[1];
    m_pOwner04 = pOwner;
    m_nDrawValue14 = nDrawValue;
    m_nDrawFlags10 = nDrawFlags;
    m_pText08 = pText;
}

// FUNCTION: LEMBALL 0x00469A50
void LevelScreenDrawEntry::InitializeFromPositionAndText(short x, short y, void *pOwner, void *pText, int nDrawFlags, int nDrawValue) {
    m_xStart1C = x;
    m_yStart1E = y;
    m_pOwner04 = pOwner;
    m_nDrawValue14 = nDrawValue;
    m_nDrawFlags10 = nDrawFlags;
    m_pText08 = pText;
}

LevelResourceScreenDrawEntry::LevelResourceScreenDrawEntry(int cchText) {
    m_cchOwnedText48 = cchText;
    m_pszOwnedText44 = (char *)AllocateVSMemBlock(cchText + 1);
}

// FUNCTION: LEMBALL 0x0046A1B0
void LevelResourceScreenDrawEntry::InitializeFromPointAndDynamicText(const short *pPoint, void *pOwner, GameDynamicCString Text, int nDrawFlags, int nDrawValue) {
    strcpy(m_pszOwnedText44, Text.m_pszText);
    LevelScreenDrawEntry::InitializeFromPointAndText(
        pPoint, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
    DestroyDynamicCString(&Text);
}

// FUNCTION: LEMBALL 0x0046A140
void LevelResourceScreenDrawEntry::InitializeFromPositionAndDynamicText(short x, short y, void *pOwner, GameDynamicCString Text, int nDrawFlags, int nDrawValue) {
    strcpy(m_pszOwnedText44, Text.m_pszText);
    LevelScreenDrawEntry::InitializeFromPositionAndText(
        x, y, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
    DestroyDynamicCString(&Text);
}

// FUNCTION: LEMBALL 0x0046A0E0
void LevelResourceScreenDrawEntry::InitializeFromPointAndText(const short *pPoint, void *pOwner, void *pText, int nDrawFlags, int nDrawValue) {
    strcpy(m_pszOwnedText44, (const char *)pText);
    LevelScreenDrawEntry::InitializeFromPointAndText(
        pPoint, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
}

// FUNCTION: LEMBALL 0x0046A080
void LevelResourceScreenDrawEntry::InitializeFromPositionAndText(short x, short y, void *pOwner, void *pText, int nDrawFlags, int nDrawValue) {
    strcpy(m_pszOwnedText44, (const char *)pText);
    LevelScreenDrawEntry::InitializeFromPositionAndText(
        x, y, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
}

// FUNCTION: LEMBALL 0x00469C60
MappedListResourceTable::MappedListResourceTable(int cResourceIds, int cSlots, int cDrawEntries, int cchOwnedText) {
    int i;

    m_nCurrentSlot18 = 0;
    m_cResourceIds0C = cResourceIds;
    m_nReserved10 = 0;
    m_cSlots08 = cSlots;
    m_apResources00 = (void **)AllocateVSMemBlock(cSlots * sizeof(void *));
    m_aiResourceSlots04 =
        (short *)AllocateVSMemBlock(cResourceIds * sizeof(short));
    for (i = 0; i < cSlots; ++i) {
        m_apResources00[i] = 0;
    }
    for (i = 0; i < cResourceIds; ++i) {
        m_aiResourceSlots04[i] = (short)cSlots;
    }
    m_cDrawEntries14 = cDrawEntries;
    m_apDrawEntries20 = (LevelScreenDrawEntry **)AllocateVSMemBlock(
        cDrawEntries * sizeof(LevelScreenDrawEntry *));
    for (i = 0; i < cDrawEntries; ++i) {
        if (cchOwnedText == 0) {
            m_apDrawEntries20[i] = new (AllocateVSMemBlock(
                sizeof(LevelScreenDrawEntry))) LevelScreenDrawEntry;
        } else {
            m_apDrawEntries20[i] = new (AllocateVSMemBlock(
                sizeof(LevelResourceScreenDrawEntry)))
                    LevelResourceScreenDrawEntry(cchOwnedText);
        }
    }
    m_nCurrentSlot18 = 0;
}

DECOMP_SIZE_ASSERT(LevelQueuedRenderPointSink, 0x04);
DECOMP_SIZE_ASSERT(LevelPackagedRectQueueEntry, 0x18);
DECOMP_SIZE_ASSERT(LevelScreenDrawEntry, 0x44);
DECOMP_SIZE_ASSERT(LevelResourceScreenDrawEntry, 0x4c);
DECOMP_SIZE_ASSERT(MappedListResourceTable, 0x24);
