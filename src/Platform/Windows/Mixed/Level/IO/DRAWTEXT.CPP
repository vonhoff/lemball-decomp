#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"
#include "Platform/Windows/Mixed/Engine/GDI/VSGDI.H"
#include "Control/CGame.h"
#include "Platform/Windows/Mixed/Level/DRAWTEXT.H"
#include "Visos/Generic/MogLoad.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>

extern void* LoadListResource(int nResourceId);
extern void* g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[];

extern void* g_MOGLOAD_CachedResourceObjectBaseVtable[15];
extern void __fastcall InitializeResourceObjectFromId(void* pObject, void* pUnusedEdx, int nResourceId);
extern void* __fastcall FinalizeLoadedResourceObjectResult(void* pObject, void* pUnusedEdx);
extern void LEMBALL_FASTCALL InitializeTypedResourceObjectBaseVtable(void* pObject);
extern void ParseIntResourceDescriptor(void);
extern int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void* pObject);
extern int LEMBALL_FASTCALL GetField1CVfunc04(void* pObject);
extern int LEMBALL_FASTCALL GetField14Vfunc05(void* pObject);
extern int ReturnZeroVfunc06(void);
extern void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void* pObject);
extern void LEMBALL_FASTCALL UnloadTypedResourceObject(void* pObject, void* pUnusedEcx, int fReleaseMode);
extern void LEMBALL_FASTCALL NoopVfunc09(void* pObject, void* pUnusedEdx, int nArgument);
extern void* LEMBALL_FASTCALL GetEffResourceDataPointer(void* pObject);
extern void LEMBALL_FASTCALL CopyBufferIntoTypedResourceObjectAndParse(void* pObject,
																	   void* pUnusedEdx,
																	   unsigned int* pSource,
																	   unsigned int nUnused,
																	   unsigned int cbBuffer);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern int LEMBALL_FASTCALL GetField28GetMemorySize(void* pObject);

void LEMBALL_FASTCALL SetBoolResourceTypeTag(void* pObject);
void LEMBALL_FASTCALL ReadBoolResourcePayloadFromStream(void* pObject);
void* LEMBALL_FASTCALL DeleteBoolResource(void* pObject, int nUnused, int fDelete);

static const unsigned int g_DRAWTEXT_BoolResourceTypeTag = 0x424f4f4c;

class LevelGlyphListView {
public:
	virtual void* GetGlyphListResource(void) = 0;
	virtual void* FindGlyph(unsigned int ch) = 0;
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
	virtual void DrawGlyph(LevelPackagedRectQueueEntry* pEntry, void* pGlyph) = 0;
};

struct LevelScreenDrawOwnerView {
	void** m_pVtable00;
	int m_nReserved04;
	int m_cQueuedEntries08;
	int m_nReserved0C;
	int m_fImmediateQueue10;
	char m_abReserved14[0x10];
	int m_nQueueState24;
	char m_abReserved28[8];
	unsigned int m_nFontResourceId30;
	char m_abReserved34[0x44];
	LevelGlyphListView* m_pGlyphList78;
	void* m_pFallbackGlyph7C;

	void* LookupGlyph(unsigned int ch);
	short* MeasureGlyphStringExtent(short* pExtent, const char* pszText, unsigned int nFlags);
};

struct LevelDrawContextView {
	char m_abReserved00[0x0c];
	int m_nDispatchOffset0C;
};

// FUNCTION: LEMBALL 0x0045bad0
VsInitFormattedOutputStream* AppendRNAMEToStream(VsInitFormattedOutputStream* pStream, unsigned int nValue)
{
	pStream->AppendIntToStream(nValue);
	return pStream;
}
// FUNCTION: LEMBALL 0x0045db20
void* LevelScreenDrawOwnerView::LookupGlyph(unsigned int ch)
{
	return m_pGlyphList78->FindGlyph(ch);
}

// FUNCTION: LEMBALL 0x0045db30
short* LevelScreenDrawOwnerView::MeasureGlyphStringExtent(short* pExtent, const char* pszText, unsigned int nFlags)
{
	int i;
	short cx;
	short cy;

	i = 0;
	cx = 0;
	cy = 0;
	while (pszText[i] != '\0') {
		char* pGlyph = (char*) LookupGlyph((unsigned char) pszText[i]);
		if (pGlyph == 0) {
			pGlyph = (char*) LookupGlyph(0x49);
			if (pGlyph == 0) {
				pGlyph = (char*) m_pFallbackGlyph7C;
			}
		}
		if ((nFlags & 0x60) == 0) {
			int nWidth = *(short*) (pGlyph + 0x48) + *(short*) (pGlyph + 0x4c);
			if (cx < nWidth) {
				cx = (short) nWidth;
			}
		}
		else {
			cx = (short) (cx + *(short*) (pGlyph + 0x4c) + 1);
		}
		if ((nFlags & 0x180) == 0) {
			int nHeight = *(short*) (pGlyph + 0x4a) + *(short*) (pGlyph + 0x4e);
			if (cy < nHeight) {
				cy = (short) nHeight;
			}
		}
		else {
			cy = (short) (cy + *(short*) (pGlyph + 0x4e) + 1);
		}
		++i;
	}
	if ((nFlags & 0x60) != 0) {
		--cx;
	}
	if ((nFlags & 0x180) != 0) {
		--cy;
	}
	pExtent[0] = cx;
	pExtent[1] = cy;
	return pExtent;
}

// GLOBAL: LEMBALL 0x00498b88
static void* g_DRAWTEXT_BoolResourceVtableStorage[15] = {
	(void*) DeleteBoolResource,
	(void*) CopyBufferIntoTypedResourceObjectAndParse,
	(void*) ParseIntResourceDescriptor,
	(void*) GetField14NeedsAgeIncrement,
	(void*) GetField1CVfunc04,
	(void*) GetField14Vfunc05,
	(void*) ReturnZeroVfunc06,
	(void*) EnsureTypedResourceObjectLoaded,
	(void*) UnloadTypedResourceObject,
	(void*) NoopVfunc09,
	(void*) GetEffResourceDataPointer,
	(void*) ReadBoolResourcePayloadFromStream,
	(void*) NoopVfunc12,
	(void*) SetBoolResourceTypeTag,
	(void*) GetField28GetMemorySize,
};
static void* g_pBoolResourceVtable = g_DRAWTEXT_BoolResourceVtableStorage;

// FUNCTION: LEMBALL 0x0045dc40
int* load_bool_resource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject = (int*) ((CMogRes*) g_pResourceArchive)->Find(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_DRAWTEXT_BoolResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x4c);
	if (pResourceObject != 0) {
		*(void**) pResourceObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pResourceObject[6] = 0;
		*(void**) pResourceObject = g_DRAWTEXT_BoolResourceVtableStorage;
		InitializeResourceObjectFromId(pResourceObject, 0, nResourceId);
		return (int*) FinalizeLoadedResourceObjectResult(pResourceObject, 0);
	}
	return (int*) FinalizeLoadedResourceObjectResult(0, 0);
}

// Int resource
static const unsigned int g_DRAWTEXT_IntResourceTypeTag = 0x494e5420;

extern void LEMBALL_FASTCALL SetIntResourceTypeTag(void* pResource);
extern void LEMBALL_FASTCALL ReadIntResourcePayloadFromStream(void* pResource);
extern void* LEMBALL_FASTCALL DestroyIntResourceEntryArray(void* pObject, int nUnused, unsigned int fDelete);
extern void LEMBALL_FASTCALL ResetTypedResourceObjectState(void* pObject);

// GLOBAL: LEMBALL 0x00498b48
static void* g_DRAWTEXT_IntResourceVtableStorage[15] = {
	(void*) DestroyIntResourceEntryArray,
	(void*) CopyBufferIntoTypedResourceObjectAndParse,
	(void*) ParseIntResourceDescriptor,
	(void*) GetField14NeedsAgeIncrement,
	(void*) GetField1CVfunc04,
	(void*) GetField14Vfunc05,
	(void*) ReturnZeroVfunc06,
	(void*) EnsureTypedResourceObjectLoaded,
	(void*) UnloadTypedResourceObject,
	(void*) NoopVfunc09,
	(void*) GetEffResourceDataPointer,
	(void*) ReadIntResourcePayloadFromStream,
	(void*) NoopVfunc12,
	(void*) SetIntResourceTypeTag,
	(void*) GetField28GetMemorySize,
};

// FUNCTION: LEMBALL 0x0045dcb0
int* load_int_resource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject = (int*) ((CMogRes*) g_pResourceArchive)->Find(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_DRAWTEXT_IntResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x4c);
	if (pResourceObject != 0) {
		*(void**) pResourceObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pResourceObject[6] = 0;
		*(void**) pResourceObject = g_DRAWTEXT_IntResourceVtableStorage;
		InitializeResourceObjectFromId(pResourceObject, 0, nResourceId);
		return (int*) FinalizeLoadedResourceObjectResult(pResourceObject, 0);
	}
	return (int*) FinalizeLoadedResourceObjectResult(0, 0);
}

// FUNCTION: LEMBALL 0x0045e8f0
void* LEMBALL_FASTCALL ConstructListIntEntryBase(void* pObject)
{
	*(void* volatile*) pObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
	*(void* volatile*) pObject = g_DRAWTEXT_IntResourceVtableStorage;
	*(int*) ((char*) pObject + 0x18) = 0;
	ResetTypedResourceObjectState(pObject);
	return pObject;
}

// FUNCTION: LEMBALL 0x0045e9b0
void LEMBALL_FASTCALL SetBoolResourceTypeTag(void* pObject)
{
	*(unsigned int*) ((char*) pObject + 0x40) = g_DRAWTEXT_BoolResourceTypeTag;
}

// FUNCTION: LEMBALL 0x0045e9c0
void LEMBALL_FASTCALL ReadBoolResourcePayloadFromStream(void* pObject)
{
	*(int*) ((char*) pObject + 0x48) = **(int**) ((char*) pObject + 0x38);
}

// FUNCTION: LEMBALL 0x0045e9d0
void* LEMBALL_FASTCALL DeleteBoolResource(void* pObject, int nUnused, int fDelete)
{
	(void) nUnused;
	InitializeTypedResourceObjectBaseVtable(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00467ae0
void LevelPackagedRectQueueEntry::AppendToQueue(void* pQueue)
{
	int* pOwner;

	pOwner = (int*) m_pOwner08;
	if (pOwner[4] == 0) {
		((void(LEMBALL_FASTCALL*)(void*, int))(*(void***) pOwner)[7])(pOwner, 0);
	}
	else {
		pOwner[9] = 0;
	}
	++pOwner[2];
	((VsGdiHelperDispatchQueue*) pQueue)->AppendPointerQueueEntry(this);
}

// FUNCTION: LEMBALL 0x00467b10
void LevelPackagedRectQueueEntry::Process(void* pContext)
{
	int* pOwner;
	int pOwnerOffsets;
	int pDispatchBase;
	void* pDispatchObject;

	pOwner = (int*) m_pOwner08;
	pOwnerOffsets = *(int*) (*(int*) ((char*) pContext + 0x0c) + 0x40);
	pDispatchBase = *(int*) (pOwnerOffsets + 4);
	pDispatchObject = (void*) (unsigned long) (pDispatchBase + 0x40 + *(int*) ((char*) pContext + 0x0c));
	((void(LEMBALL_FASTCALL*)(void*, int, void*, void*))(
		*(void***) pDispatchObject)[9])(pDispatchObject, 0, this, pOwner);
	--pOwner[2];
}

// FUNCTION: LEMBALL 0x00467b40
void* LevelPackagedRectQueueEntry::Delete(unsigned int fDelete)
{
	if ((fDelete & 2) != 0) {
		int i;
		char* pAllocation;
		LevelPackagedRectQueueEntry* pEntry;

		pAllocation = (char*) this - 4;
		i = *(int*) pAllocation;
		pEntry = this + i;
		while (--i >= 0) {
			(--pEntry)->ResetVtableAfterDestruction();
		}
		FreeVSMemBlock(pAllocation);
		return this;
	}

	*(void**) this = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(this);
	}
	return this;
}

// FUNCTION: LEMBALL 0x00467ba0
void LevelPackagedRectQueueEntry::ResetVtableAfterDestruction()
{
	*(void**) this = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00469a00
LevelScreenDrawEntry::LevelScreenDrawEntry()
{
	m_xCurrent0C = 0;
	m_yCurrent0E = 0;
	m_nReserved18 = 0;
	m_nReserved1A = 0;
	m_xStart1C = 0;
	m_yStart1E = 0;
	m_nReserved24 = 0;
	m_nReserved26 = 0;
}

// FUNCTION: LEMBALL 0x00469a50
void LevelScreenDrawEntry::InitializeFromPositionAndText(int x,
														 int y,
														 void* pOwner,
														 void* pText,
														 int nDrawFlags,
														 int nDrawValue)
{
	m_xStart1C = x;
	m_yStart1E = y;
	m_pOwner04 = pOwner;
	m_nDrawValue14 = nDrawValue;
	m_nDrawFlags10 = nDrawFlags;
	m_pText08 = pText;
}

// FUNCTION: LEMBALL 0x00469a80
void LevelScreenDrawEntry::InitializeFromPointAndText(const short* pPoint,
													  void* pOwner,
													  void* pText,
													  int nDrawFlags,
													  int nDrawValue)
{
	m_xStart1C = pPoint[0];
	m_yStart1E = pPoint[1];
	m_pOwner04 = pOwner;
	m_nDrawValue14 = nDrawValue;
	m_nDrawFlags10 = nDrawFlags;
	m_pText08 = pText;
}

// FUNCTION: LEMBALL 0x00469ac0
void LevelScreenDrawEntry::InitializeFromPositionAndDynamicText(int x,
																int y,
																void* pOwner,
																CString Text,
																int nDrawFlags,
																int nDrawValue)
{
	m_xStart1C = x;
	m_yStart1E = y;
	m_pOwner04 = pOwner;
	m_nDrawFlags10 = nDrawFlags;
	m_nDrawValue14 = nDrawValue;
	m_pText08 = Text.m_pszText;
	Text.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x00469b00
void LevelScreenDrawEntry::InitializeFromPointAndDynamicText(const short* pPoint,
															 void* pOwner,
															 CString Text,
															 int nDrawFlags,
															 int nDrawValue)
{
	m_xStart1C = pPoint[0];
	m_yStart1E = pPoint[1];
	m_pOwner04 = pOwner;
	m_nDrawFlags10 = nDrawFlags;
	m_nDrawValue14 = nDrawValue;
	m_pText08 = Text.m_pszText;
	Text.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x00469b40
void LevelScreenDrawEntry::AppendToQueue(void* pQueue)
{
	m_nQueueState20 = 0;
	((VsGdiHelperDispatchQueue*) pQueue)->AppendPointerQueueEntry(this);
}

// FUNCTION: LEMBALL 0x00469b80
void* LevelScreenDrawEntry::Delete(unsigned int fDelete)
{
	if ((fDelete & 2) != 0) {
		int i;
		char* pAllocation;
		LevelScreenDrawEntry* pEntry;

		pAllocation = (char*) this - 4;
		i = *(int*) pAllocation;
		pEntry = this + i;
		while (--i >= 0) {
			(--pEntry)->ResetVtablesAfterDestruction();
		}
		FreeVSMemBlock(pAllocation);
		return this;
	}

	*(void**) this = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	*(void**) ((char*) this + 0x2c) = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(this);
	}
	return this;
}

// FUNCTION: LEMBALL 0x00469bf0
void LevelScreenDrawEntry::ResetVtablesAfterDestruction()
{
	*(void**) ((char*) this + 0x2c) = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	*(void**) this = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00469c60
CTextManager::CTextManager(int cResourceIds, int cSlots, int cDrawEntries, int cchOwnedText)
{
	int i;

	m_nCurrentSlot18 = 0;
	m_cResourceIds0C = cResourceIds;
	m_nReserved10 = 0;
	m_cSlots08 = cSlots;
	m_apResources00 = (void**) AllocateVSMemBlock(cSlots * sizeof(void*));
	m_aiResourceSlots04 = (short*) AllocateVSMemBlock(cResourceIds * sizeof(short));
	for (i = 0; i < cSlots; ++i) {
		m_apResources00[i] = 0;
	}
	for (i = 0; i < cResourceIds; ++i) {
		m_aiResourceSlots04[i] = (short) cSlots;
	}
	m_cDrawEntries14 = cDrawEntries;
	m_apDrawEntries20 = (LevelScreenDrawEntry**) AllocateVSMemBlock(cDrawEntries * sizeof(LevelScreenDrawEntry*));
	for (i = 0; i < cDrawEntries; ++i) {
		if (cchOwnedText == 0) {
			m_apDrawEntries20[i] = new (AllocateVSMemBlock(sizeof(LevelScreenDrawEntry))) LevelScreenDrawEntry;
		}
		else {
			m_apDrawEntries20[i] = new (AllocateVSMemBlock(sizeof(LevelResourceScreenDrawEntry)))
				LevelResourceScreenDrawEntry(cchOwnedText);
		}
	}
	m_nCurrentSlot18 = 0;
}

DECOMP_SIZE_ASSERT(LevelQueuedRenderPointSink, 0x04);
DECOMP_SIZE_ASSERT(LevelPackagedRectQueueEntry, 0x18);
DECOMP_SIZE_ASSERT(LevelScreenDrawEntry, 0x44);
DECOMP_SIZE_ASSERT(LevelResourceScreenDrawEntry, 0x4c);
DECOMP_SIZE_ASSERT(CTextManager, 0x24);

// FUNCTION: LEMBALL 0x00469e20
void CTextManager::Destroy(void)
{
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, int fDelete);
	int i;
	int cReleased;

	cReleased = 0;
	for (i = 0; i < m_cSlots08 && cReleased < m_nReserved10; ++i) {
		if (m_apResources00[i] != 0) {
			ReleaseTypedResourceObjectReference(m_apResources00[i]);
			++cReleased;
		}
	}
	FreeVSMemBlock(m_apResources00);
	FreeVSMemBlock(m_aiResourceSlots04);
	if (m_apDrawEntries20 != 0) {
		for (i = 0; i < m_cDrawEntries14; ++i) {
			if (m_apDrawEntries20[i] != 0) {
				((DeleteProc) (*(void***) m_apDrawEntries20[i])[0])(m_apDrawEntries20[i], 0, 1);
			}
		}
		FreeVSMemBlock(m_apDrawEntries20);
	}
}

// FUNCTION: LEMBALL 0x00469eb0
void CTextManager::LoadFont(int nResourceId)
{
	int i;

	i = 0;
	while (m_apResources00[i] != 0) {
		++i;
	}
	m_apResources00[i] = LoadListResource(nResourceId);
	m_aiResourceSlots04[nResourceId] = (short) i;
	if (m_apResources00[i] != 0) {
		++m_nReserved10;
	}
}

// FUNCTION: LEMBALL 0x00469ef0
void* CTextManager::GetFont(int nResourceId)
{
	return m_apResources00[m_aiResourceSlots04[nResourceId]];
}

// FUNCTION: LEMBALL 0x00469f10
void CTextManager::UnLoadFont(int nResourceId)
{
	int i;

	i = m_aiResourceSlots04[nResourceId];
	if (m_apResources00[i] != 0) {
		ReleaseTypedResourceObjectReference(m_apResources00[i]);
		--m_nReserved10;
	}
	m_apResources00[i] = 0;
}

// FUNCTION: LEMBALL 0x00469f50
void CTextManager::DrawString(void* pRenderQueue,
												const short* pPoint,
												const short* pRelativePoint,
												int nResourceId,
												void* pText,
												unsigned int nDrawFlags,
												int nDrawValue)
{
	LevelScreenDrawEntry* pEntry;
	void* pResource;

	pEntry = m_apDrawEntries20[m_nCurrentSlot18++];
	pResource = GetFont(nResourceId);
	if (pResource == 0) {
		--m_nCurrentSlot18;
		return;
	}
	if (pRelativePoint[0] != 0 || pRelativePoint[1] != 0) {
		pEntry->m_nReserved18 = pRelativePoint[0];
		pEntry->m_nReserved1A = pRelativePoint[1];
		nDrawFlags |= 0x200;
	}
	pEntry->InitializeFromPointAndText(pPoint, pResource, pText, nDrawFlags, nDrawValue);
	pEntry->AppendToQueue(pRenderQueue);
}
// FUNCTION: LEMBALL 0x0046a080
void LevelResourceScreenDrawEntry::InitializeFromPositionAndText(int x,
																 int y,
																 void* pOwner,
																 void* pText,
																 int nDrawFlags,
																 int nDrawValue)
{
	strcpy(m_pszOwnedText44, (const char*) pText);
	LevelScreenDrawEntry::InitializeFromPositionAndText(x, y, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
}

// FUNCTION: LEMBALL 0x0046a0e0
void LevelResourceScreenDrawEntry::InitializeFromPointAndText(const short* pPoint,
															  void* pOwner,
															  void* pText,
															  int nDrawFlags,
															  int nDrawValue)
{
	strcpy(m_pszOwnedText44, (const char*) pText);
	LevelScreenDrawEntry::InitializeFromPointAndText(pPoint, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
}

// FUNCTION: LEMBALL 0x0046a140
void LevelResourceScreenDrawEntry::InitializeFromPositionAndDynamicText(int x,
																		int y,
																		void* pOwner,
																		CString Text,
																		int nDrawFlags,
																		int nDrawValue)
{
	strcpy(m_pszOwnedText44, Text.m_pszText);
	LevelScreenDrawEntry::InitializeFromPositionAndText(x, y, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
	Text.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x0046a1b0
void LevelResourceScreenDrawEntry::InitializeFromPointAndDynamicText(const short* pPoint,
																	 void* pOwner,
																	 CString Text,
																	 int nDrawFlags,
																	 int nDrawValue)
{
	strcpy(m_pszOwnedText44, Text.m_pszText);
	LevelScreenDrawEntry::InitializeFromPointAndText(pPoint, pOwner, m_pszOwnedText44, nDrawFlags, nDrawValue);
	Text.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x0046a220
void* LevelResourceScreenDrawEntry::Delete(unsigned int fDelete)
{
	FreeVSMemBlock(m_pszOwnedText44);
	*(void**) this = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	*(void**) ((char*) this + 0x2c) = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(this);
	}
	return this;
}

// FUNCTION: LEMBALL 0x004749c0
void LevelScreenDrawEntry::AdvanceCursor()
{
	short cx;
	short cy;

	if (m_nQueueState20 == 0) {
		cx = *(short*) ((char*) m_pGlyph28 + 0x4c) + 1;
		cy = *(short*) ((char*) m_pGlyph28 + 0x4e) + 1;
	}
	else {
		cx = m_nReserved24;
		cy = m_nReserved26;
	}
	if ((m_nDrawFlags10 & 0x200) != 0) {
		cx = (short) (cx + m_nReserved18);
		cy = (short) (cy + m_nReserved1A);
	}
	if ((m_nDrawFlags10 & 0x40) != 0) {
		m_xCurrent0C = (short) (m_xCurrent0C - cx);
	}
	else if ((m_nDrawFlags10 & 0x20) != 0) {
		m_xCurrent0C = (short) (m_xCurrent0C + cx);
	}
	if ((m_nDrawFlags10 & 0x80) != 0) {
		m_yCurrent0E = (short) (m_yCurrent0E - cy);
	}
	else if ((m_nDrawFlags10 & 0x100) != 0) {
		m_yCurrent0E = (short) (m_yCurrent0E + cy);
	}
}

// FUNCTION: LEMBALL 0x00474a20
void LevelScreenDrawEntry::Process(void* pRenderContext)
{
	LevelScreenDrawOwnerView* pOwner;
	const unsigned char* pch;
	LevelDrawContextView* pContext;
	int nDispatchBase;
	LevelDrawDispatchView* pDispatch;

	pOwner = (LevelScreenDrawOwnerView*) m_pOwner04;
	if (pOwner->m_fImmediateQueue10 != 0) {
		pOwner->m_nQueueState24 = 0;
	}
	else {
		((void(LEMBALL_FASTCALL*)(void*)) pOwner->m_pVtable00[7])(pOwner);
	}
	++pOwner->m_cQueuedEntries08;
	m_xCurrent0C = m_xStart1C;
	m_DrawSink2C.m_nFlags0C = m_nDrawFlags10;
	m_yCurrent0E = m_yStart1E;
	m_DrawSink2C.m_nValue10 = m_nDrawValue14;

	pch = (const unsigned char*) m_pText08;
	while (*pch != 0) {
		m_pGlyph28 = ((LevelScreenDrawOwnerView*) m_pOwner04)->LookupGlyph(*pch);
		if (m_pGlyph28 == 0) {
			m_pGlyph28 = ((LevelScreenDrawOwnerView*) m_pOwner04)->LookupGlyph(0x49);
			if (m_pGlyph28 == 0) {
				m_pGlyph28 = ((LevelScreenDrawOwnerView*) m_pOwner04)->m_pFallbackGlyph7C;
			}
			if (*pch != ' ' || m_pGlyph28 == 0) {
				AppendRNAMEToStream(g_pStatusOutputStream->AppendCStringToStream("Letter '")
											->AppendCharToStreamVariant((char) *pch)
											->AppendCStringToStream("' not found in font "),
										((LevelScreenDrawOwnerView*) m_pOwner04)->m_nFontResourceId30)
					->AppendCStringToStream("\n");
			}
			AdvanceCursor();
		}
		else {
			if ((m_nDrawFlags10 & 0xc0) != 0) {
				AdvanceCursor();
			}
			m_DrawSink2C.m_x04 = m_xCurrent0C;
			m_DrawSink2C.m_y06 = m_yCurrent0E;
			pContext = (LevelDrawContextView*) pRenderContext;
			nDispatchBase = *(int*) (*(int*) (pContext->m_nDispatchOffset0C + 0x40) + 4);
			pDispatch = (LevelDrawDispatchView*) (unsigned long) (nDispatchBase + pContext->m_nDispatchOffset0C + 0x40);
			pDispatch->DrawGlyph(&m_DrawSink2C, m_pGlyph28);
			if ((m_nDrawFlags10 & 0xc0) == 0) {
				AdvanceCursor();
			}
		}
		++pch;
	}
	--((LevelScreenDrawOwnerView*) m_pOwner04)->m_cQueuedEntries08;
}

struct PackagedRectQueueAllocatorView {
	void* AllocateNextPackagedRectQueueEntry(void);
};

void* PackagedRectQueueAllocatorView::AllocateNextPackagedRectQueueEntry(void)
{
	int nIndex;
	void* pEntry;

	nIndex = *(int*) ((char*) this + 0x0c);
	pEntry = (char*) *(void**) ((char*) this + 4) + (*(int*) ((char*) this + 0x50) + nIndex) * 0x10;
	nIndex += 2;
	*(int*) ((char*) this + 0x0c) = nIndex;
	return pEntry;
}
