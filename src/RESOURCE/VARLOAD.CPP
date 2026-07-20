#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "GAME.H"
#include "RESOURCE/MOGLOAD.H"
#include "RESOURCE/TYPEDRES.H"
#include "RESOURCE/VARLOAD.H"

extern void LEMBALL_FASTCALL CopyBufferIntoTypedResourceObjectAndParse(void* pObject,
																	   void* pUnusedEdx,
																	   unsigned int* pSource,
																	   unsigned int nUnused,
																	   unsigned int cbBuffer);
extern int two_array_list_is_first_entry_loaded_false(void);
extern void PASCAL two_array_list_dispatch_entry_noop(void* pUnused1, void* pUnused2);
extern int PASCAL two_array_list_has_no_entry_descriptor(void* pUnused);

static const unsigned int g_GAME_ListResourceTypeTag = 0x4c495354;
static const unsigned int g_GAME_BitmapResourceTypeTag = 0x42544d50;
static const unsigned int g_GAME_PaletteResourceTypeTag = 0x50414c20;
static const unsigned int g_GAME_ZrleResourceTypeTag = 0x5a524c45;

extern void LEMBALL_FASTCALL InitializeTypedResourceObjectBaseVtable(void* pObject);
extern void ResetTypedResourceObjectState(void* pObject);
extern void ParseIntResourceDescriptor(void);
extern void LEMBALL_FASTCALL UnloadTypedResourceObject(void* pObject, void* pUnusedEcx, int fReleaseMode);
extern void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void* pObject);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);
extern int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void* pObject);
extern int LEMBALL_FASTCALL GetField1CVfunc04(void* pObject);
extern int LEMBALL_FASTCALL GetField14Vfunc05(void* pObject);
extern int ReturnZeroVfunc06(void);
extern void LEMBALL_FASTCALL NoopVfunc09(void* pObject, void* pUnusedEdx, int nArgument);
extern void* LEMBALL_FASTCALL GetEffResourceDataPointer(void* pObject);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern void LEMBALL_FASTCALL ClearResourceTypeTag(void* pObject);
extern int LEMBALL_FASTCALL GetField28GetMemorySize(void* pObject);

void LEMBALL_FASTCALL PopulateZrleResourceDescriptorFields(void* pObject);
void LEMBALL_FASTCALL SetupZrleResourceType(void* pResource);
void* LEMBALL_FASTCALL DestroyZrleResourceEntryArray(void* pObject, int nUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DestroyIntResourceEntryArray(void* pObject, int nUnused, unsigned int fDelete);
void LEMBALL_FASTCALL ReadIntResourcePayloadFromStream(void* pResource);
void LEMBALL_FASTCALL SetIntResourceTypeTag(void* pResource);

static void* g_GAME_ZrleResourceVtableStorage[15] = {
	(void*) DestroyZrleResourceEntryArray,
	(void*) CopyBufferIntoTypedResourceObjectAndParse,
	(void*) PopulateZrleResourceDescriptorFields,
	(void*) GetField14NeedsAgeIncrement,
	(void*) GetField1CVfunc04,
	(void*) GetField14Vfunc05,
	(void*) ReturnZeroVfunc06,
	(void*) EnsureTypedResourceObjectLoaded,
	(void*) UnloadTypedResourceObject,
	(void*) NoopVfunc09,
	(void*) GetEffResourceDataPointer,
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetupZrleResourceType,
	(void*) GetField28GetMemorySize,
};

extern void* g_pDestroyGRTSResourceAndFreeThunk;
extern void* LEMBALL_FASTCALL DestroyGRTSResourceAndFree(void* pObject, int nUnusedEdx, int fDelete);
extern void* g_pStringResourceVtable;
extern void* g_MOGLOAD_CachedResourceObjectBaseVtable[15];
extern void* g_pBitmapResourceVtable;
extern void* g_pPaletteResourceVtable;

struct GameVariantResourceBundleLoadInterface {
	virtual void PrepareNextResourceLoad(void) = 0;
};

struct GameListEntryArrayDeleteInterface {
	virtual void* Delete(int fDelete) = 0;
};

// LIST resource objects use the same typed-resource prefix, then keep their
// descriptor-derived arrays at +0x38/+0x4c and list state at +0x60..+0x74.
void* LEMBALL_FASTCALL DeleteListResourceBase(void* pObject, int nUnused, int fDelete);
void* LEMBALL_FASTCALL DestroyZrleOnlyListResource(void* pObject, int nUnused, int fDelete);
void LEMBALL_FASTCALL DestroyTwoArrayListResource(void* pObject);
void* LEMBALL_FASTCALL DestroyTwoArrayListResourceAndFree(void* pObject, int nUnused, int fDelete);
void LEMBALL_FASTCALL CopyBuffersIntoTwoArrayListResourceAndParse(void* pObject,
																  void* pUnusedEdx,
																  unsigned int* pSource,
																  int pRelatedObject,
																  unsigned int cbBuffer);
void LEMBALL_FASTCALL ParseListResourceDescriptor(void* pObject);
int LEMBALL_FASTCALL ListNeedsAgeIncrement(void* pObject);
int LEMBALL_FASTCALL ListField68Nonzero(void* pObject);
int LEMBALL_FASTCALL ListField60Nonzero(void* pObject);
int LEMBALL_FASTCALL EnsureIntZrleListEntriesLoaded(void* pObject);
void LEMBALL_FASTCALL LoadListEntryArraysFromArchive(void* pObject);
void LEMBALL_FASTCALL ReleaseListEntriesAndStreamBuffer(void* pObject, void* pUnusedEdx, int fReleaseMode);
void LEMBALL_FASTCALL LoadListEntriesFromStreamDescriptors(void* pObject, void* pUnused, int nArgument);
void LEMBALL_FASTCALL SetListResourceTypeList(void* pObject);
int LEMBALL_FASTCALL GetListEntryArrayCursor(void* pObject);
static void* g_GAME_ListResourceVtableStorage[15] = {
	(void*) DeleteListResourceBase,
	(void*) CopyBuffersIntoTwoArrayListResourceAndParse,
	(void*) ParseListResourceDescriptor,
	(void*) ListNeedsAgeIncrement,
	(void*) ListField68Nonzero,
	(void*) ListField60Nonzero,
	(void*) EnsureIntZrleListEntriesLoaded,
	(void*) LoadListEntryArraysFromArchive,
	(void*) ReleaseListEntriesAndStreamBuffer,
	(void*) LoadListEntriesFromStreamDescriptors,
	(void*) GetEffResourceDataPointer,
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetListResourceTypeList,
	(void*) GetListEntryArrayCursor,
};
void LoadMainGameVariantZrleListResource(void* pBundle, int nResourceId);
void LoadMainGameVariantListResource(void* pBundle, int nResourceId);
void LoadMainGameVariantBitmapResource(void* pBundle, int nResourceId);
void LoadMainGameVariantPaletteResource(void* pBundle, int nResourceId);
void LoadMainGameVariantTwoArrayListResource(void* pBundle, int nResourceId);
void LEMBALL_FASTCALL ParseListResourceDescriptor(void* pObject);
void LEMBALL_FASTCALL CopyBuffersIntoTwoArrayListResourceAndParse(void* pObject,
																  void* pUnusedEdx,
																  unsigned int* pSource,
																  int pRelatedObject,
																  unsigned int cbBuffer);
void LEMBALL_FASTCALL LoadListEntryArraysFromArchive(void* pObject);
int LEMBALL_FASTCALL EnsureIntZrleListEntriesLoaded(void* pObject);
void LEMBALL_FASTCALL ReleaseListEntriesAndStreamBuffer(void* pObject, void* pUnusedEdx, int fReleaseMode);
void LEMBALL_FASTCALL LoadListEntriesFromStreamDescriptors(void* pObject, void* pUnused, int nArgument);
void* LoadZrleOnlyListResource(int nResourceId);
void* ConstructIntZrleListResourceFromId(void* pObject, int nResourceId);
void* LoadListResource(int nResourceId);
void* LoadPalResource(int nResourceId);
void* LoadTwoArrayListResource(int nResourceId);
void* ConstructTwoArrayListResourceFromId(void* pObject, int nResourceId);
void LEMBALL_FASTCALL DestroyTwoArrayListResource(void* pObject);
void* LoadBitmapResource(int nResourceId);
void* LoadZrleResource(int nResourceId);
void LEMBALL_FASTCALL PopulateZrleResourceDescriptorFields(void* pObject);
void LEMBALL_FASTCALL SetListResourceTypeList(void* pObject);
int LEMBALL_FASTCALL GetListEntryArrayCursor(void* pObject);
void InitializeZrleOnlyListEntry(void* pEntry);
void LEMBALL_FASTCALL AllocateZrleOnlyListEntryArray(void* pObject, int, int cEntries);
void LEMBALL_FASTCALL DispatchZrleOnlyListEntryMethod(void* pObject, int, int iEntry, int nArgument);
int LEMBALL_FASTCALL HasZrleOnlyListEntryDescriptor(void* pObject, int, int iEntry);
void LEMBALL_FASTCALL LoadZrleOnlyListEntryFromStream(void* pObject, int, int iEntry, int* pDescriptor);
void LEMBALL_FASTCALL
LoadZrleOnlyListEntryFromBuffer(void* pObject, int, int iEntry, int* pDescriptorCursor, int* pDataCursor);
void LEMBALL_FASTCALL ReleaseZrleOnlyListEntry(void* pObject, int, int iEntry, int fReleaseMode);
void LEMBALL_FASTCALL AllocateTwoArrayListEntryArrays(void* pObject, int nUnusedEdx, int cEntries);
int LEMBALL_FASTCALL LoadTwoArrayListEntryPairFromBuffers(void* pObject,
														  int nUnusedEdx,
														  int iEntry,
														  int* pDescriptorCursor,
														  int* pDataCursor);
int LEMBALL_FASTCALL LoadTwoArrayListEntryPairFromStream(void* pObject, int nUnusedEdx, int iEntry, int* pDataCursor);
void LEMBALL_FASTCALL ReleaseTwoArrayListEntryPair(void* pObject, int nUnusedEdx, int iEntry, int fReleaseMode);
int LEMBALL_FASTCALL ListNeedsAgeIncrement(void* pObject);
int LEMBALL_FASTCALL ListField60Nonzero(void* pObject);
int LEMBALL_FASTCALL ListField68Nonzero(void* pObject);
void* LEMBALL_FASTCALL DeleteListResourceBase(void* pObject, int nUnused, int fDelete);
int LEMBALL_FASTCALL ListIsFirstEntryLoaded(void* pResource);
void* LEMBALL_FASTCALL DestroyZrleOnlyListResource(void* pObject, int, int fDelete);
void LEMBALL_FASTCALL SetupZrleResourceType(void* pResource);
void* LEMBALL_FASTCALL DestroyZrleResourceEntryArray(void* pObject, int nUnused, unsigned int fDelete);
void LEMBALL_FASTCALL SetIntResourceTypeTag(void* pResource);
void LEMBALL_FASTCALL ReadIntResourcePayloadFromStream(void* pResource);
void LEMBALL_FASTCALL SetPalResourceType(void* pResource);
void* LEMBALL_FASTCALL DestroyTwoArrayListResourceAndFree(void* pObject, int nUnused, int fDelete);
void LEMBALL_FASTCALL SetGamiResourceTypeTag(void* pResource);
void LEMBALL_FASTCALL SetBitmapResourceType(void* pResource);
void LEMBALL_FASTCALL SetEffResourceTypeTag(void* pResource);
void LEMBALL_FASTCALL SetPtchResourceTypeTag(void* pResource);
void LEMBALL_FASTCALL SetTuneResourceTypeTag(void* pResource);
void LEMBALL_FASTCALL SetPresResourceTypeTag(void* pResource);

// GLOBAL: LEMBALL 0x00498a18
static void* g_GAME_ZrleOnlyListResourceVtableStorage[22] = {
	(void*) DestroyZrleOnlyListResource,
	(void*) CopyBuffersIntoTwoArrayListResourceAndParse,
	(void*) ParseListResourceDescriptor,
	(void*) ListNeedsAgeIncrement,
	(void*) ListField68Nonzero,
	(void*) ListField60Nonzero,
	(void*) EnsureIntZrleListEntriesLoaded,
	(void*) LoadListEntryArraysFromArchive,
	(void*) ReleaseListEntriesAndStreamBuffer,
	(void*) LoadListEntriesFromStreamDescriptors,
	(void*) GetEffResourceDataPointer,
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetListResourceTypeList,
	(void*) GetListEntryArrayCursor,
	(void*) HasZrleOnlyListEntryDescriptor,
	(void*) DispatchZrleOnlyListEntryMethod,
	(void*) AllocateZrleOnlyListEntryArray,
	(void*) ListIsFirstEntryLoaded,
	(void*) LoadZrleOnlyListEntryFromBuffer,
	(void*) LoadZrleOnlyListEntryFromStream,
	(void*) ReleaseZrleOnlyListEntry,
};

static void* g_GAME_IntResourceVtableStorage[15] = {
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
static void* g_GAME_TwoArrayListResourceVtableStorage[24] = {
	(void*) DestroyTwoArrayListResourceAndFree,
	(void*) CopyBuffersIntoTwoArrayListResourceAndParse,
	(void*) ParseListResourceDescriptor,
	(void*) ListNeedsAgeIncrement,
	(void*) ListField68Nonzero,
	(void*) ListField60Nonzero,
	(void*) EnsureIntZrleListEntriesLoaded,
	(void*) LoadListEntryArraysFromArchive,
	(void*) ReleaseListEntriesAndStreamBuffer,
	(void*) LoadListEntriesFromStreamDescriptors,
	(void*) GetEffResourceDataPointer,
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetListResourceTypeList,
	(void*) GetListEntryArrayCursor,
	(void*) two_array_list_has_no_entry_descriptor,
	(void*) two_array_list_dispatch_entry_noop,
	(void*) AllocateTwoArrayListEntryArrays,
	(void*) two_array_list_is_first_entry_loaded_false,
	(void*) LoadTwoArrayListEntryPairFromBuffers,
	(void*) LoadTwoArrayListEntryPairFromStream,
	(void*) ReleaseTwoArrayListEntryPair,
	0,
	0,
};
static void* g_GAME_ListResourceBaseVtable = g_GAME_ListResourceVtableStorage;
static void* g_GAME_ZrleOnlyListResourceVtable = g_GAME_ZrleOnlyListResourceVtableStorage;
static void* g_GAME_IntZrleListResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void* g_GAME_ZrleResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void* g_GAME_TwoArrayListResourceVtable = g_GAME_TwoArrayListResourceVtableStorage;

extern void* g_pMainResourceArchive;
extern void* g_pResourceArchive;
extern void InitializeResourceObjectFromId(void* pObject, int nResourceId);
extern void* FinalizeLoadedResourceObjectResult(void* pObject);

struct GameListResourceStreamInterface {
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved0C(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual int PrepareListEntryStream(void) = 0;
	virtual void Reserved18(void) = 0;
	virtual void Reserved1C(void) = 0;
	virtual void Reserved20(void) = 0;
	virtual void Reserved24(void) = 0;
	virtual void Reserved28(void) = 0;
	virtual void Reserved2C(void) = 0;
	virtual void Reserved30(void) = 0;
	virtual void Reserved34(void) = 0;
	virtual void Reserved38(void) = 0;
	virtual void Reserved3C(void) = 0;
	virtual void LoadListEntryFromStream(unsigned int nIndex, int nArgument) = 0;
};

#include "ENGINE/CORE/SAFEVT.H"

// FUNCTION: LEMBALL 0x00447db0
void LoadMainGameVariantZrleListResource(void* pBundle, int nResourceId)
{
	GameMainGameVariantResourceBundleLoader* pResourceBundle;

	pResourceBundle = (GameMainGameVariantResourceBundleLoader*) pBundle;
	((GameVariantResourceBundleLoadInterface*) pBundle)->PrepareNextResourceLoad();
	pResourceBundle->m_ppZrleListResources[pResourceBundle->m_cLoadedZrleListResources] =
		LoadZrleOnlyListResource(nResourceId);
	++pResourceBundle->m_cLoadedZrleListResources;
}

// FUNCTION: LEMBALL 0x00447e30
void LoadMainGameVariantListResource(void* pBundle, int nResourceId)
{
	GameMainGameVariantResourceBundleLoader* pResourceBundle;

	pResourceBundle = (GameMainGameVariantResourceBundleLoader*) pBundle;
	((GameVariantResourceBundleLoadInterface*) pBundle)->PrepareNextResourceLoad();
	pResourceBundle->m_ppListResources[pResourceBundle->m_cLoadedListResources] = LoadListResource(nResourceId);
	++pResourceBundle->m_cLoadedListResources;
}

// FUNCTION: LEMBALL 0x00447eb0
void LoadMainGameVariantBitmapResource(void* pBundle, int nResourceId)
{
	GameMainGameVariantResourceBundleLoader* pResourceBundle;

	pResourceBundle = (GameMainGameVariantResourceBundleLoader*) pBundle;
	((GameVariantResourceBundleLoadInterface*) pBundle)->PrepareNextResourceLoad();
	pResourceBundle->m_ppBitmapResources[pResourceBundle->m_cLoadedBitmapResources] = LoadBitmapResource(nResourceId);
	++pResourceBundle->m_cLoadedBitmapResources;
}

// FUNCTION: LEMBALL 0x00447f30
void LoadMainGameVariantPaletteResource(void* pBundle, int nResourceId)
{
	GameMainGameVariantResourceBundleLoader* pResourceBundle;

	pResourceBundle = (GameMainGameVariantResourceBundleLoader*) pBundle;
	((GameVariantResourceBundleLoadInterface*) pBundle)->PrepareNextResourceLoad();
	pResourceBundle->m_ppPaletteResources[pResourceBundle->m_cPaletteResources] = LoadPalResource(nResourceId);
	++pResourceBundle->m_cPaletteResources;
}

// FUNCTION: LEMBALL 0x00447fb0
void GameMainGameVariantResourceBundleLoader::LoadMainGameVariantStringResource(int nResourceId)
{
	((GameVariantResourceBundleLoadInterface*) this)->PrepareNextResourceLoad();
	m_ppStringResources[m_cStringResources] = LoadStringResource(nResourceId);
	++m_cStringResources;
}

// FUNCTION: LEMBALL 0x00448030
void LoadMainGameVariantTwoArrayListResource(void* pBundle, int nResourceId)
{
	GameMainGameVariantResourceBundleLoader* pResourceBundle;

	pResourceBundle = (GameMainGameVariantResourceBundleLoader*) pBundle;
	((GameVariantResourceBundleLoadInterface*) pBundle)->PrepareNextResourceLoad();
	pResourceBundle->m_ppTwoArrayListResources[pResourceBundle->m_cTwoArrayListResources] =
		LoadTwoArrayListResource(nResourceId);
	++pResourceBundle->m_cTwoArrayListResources;
}

// FUNCTION: LEMBALL 0x0045cf70
int MogLoadTypedResourceObject::LoadTypedResourceObjectFromStreamDescriptor(int* pDataCursor, void* pOwner)
{
	void** pVtable;

	m_nReserved04 = (int) pOwner;
	m_pszText38 = (char*) *pDataCursor;
	*pDataCursor += m_cbResourceData28;
	pVtable = m_pVtable;
	++m_cReferences;
	++m_nLockCount08;
	m_nReserved24 = 0;
	m_nLoadState10 = 1;
	m_nReserved24 = 0;
	((void(LEMBALL_FASTCALL*)(void*)) pVtable[0x0b])(this);
	return 0;
}

// FUNCTION: LEMBALL 0x0045cfb0
int MogLoadTypedResourceObject::LoadTypedResourceObjectFromBufferDescriptor(int* pDescriptorCursor,
																			int* pDataCursor,
																			void* pOwner)
{
	int* pEntry;
	int cbResourceData;
	unsigned int* pResourceDescriptor;
	void** pVtable;

	m_nReserved04 = (int) pOwner;
	pEntry = (int*) *pDescriptorCursor;
	if ((int) m_uTypeTag != pEntry[0]) {
		m_nResultCode44 = 1;
		return 1;
	}
	cbResourceData = pEntry[1];
	pResourceDescriptor = (unsigned int*) (pEntry + 2);
	pVtable = m_pVtable;
	m_cbResourceData28 = cbResourceData;
	m_padwResourceDescriptor34 = pResourceDescriptor;
	((void(LEMBALL_FASTCALL*)(void*)) pVtable[2])(this);
	*pDescriptorCursor = (int) pEntry + m_nReserved3C + 8;
	m_pszText38 = (char*) *pDataCursor;
	*pDataCursor += m_cbResourceData28;
	++m_cReferences;
	++m_nLockCount08;
	m_nReserved24 = 0;
	m_nLoadState10 = 1;
	m_nReserved24 = 0;
	((void(LEMBALL_FASTCALL*)(void*)) pVtable[0x0b])(this);
	return 0;
}

// FUNCTION: LEMBALL 0x0045d290
void LEMBALL_FASTCALL ParseListResourceDescriptor(void* pObject)
{
	unsigned int* pDescriptor;

	pDescriptor = *(unsigned int**) ((char*) pObject + 0x34);
	*(unsigned int*) ((char*) pObject + 0x6c) = pDescriptor[0];
	*(unsigned int*) ((char*) pObject + 0x70) = pDescriptor[1];
	*(unsigned int*) ((char*) pObject + 0x74) = pDescriptor[2];
	*(unsigned int*) ((char*) pObject + 0x64) = 0xffffffff;
}

// FUNCTION: LEMBALL 0x0045d2b0
void LEMBALL_FASTCALL CopyBuffersIntoTwoArrayListResourceAndParse(void* pObject,
																  void* pUnusedEdx,
																  unsigned int* pSource,
																  int pRelatedObject,
																  unsigned int cbBuffer)

{
	unsigned int pTarget;
	unsigned int i;
	char* pFrom;
	char* pTo;

	(void) pUnusedEdx;
	if ((int) ((char*) pObject - (char*) pRelatedObject) == -0x4c) {
		if (*(unsigned int*) ((char*) pObject + 0x4c) == 0) {
			pTarget =
				((MogLoadResourceArchive*) g_pMainResourceArchive)->AllocateResourceDataBufferWithEviction(cbBuffer);
			*(unsigned int*) ((char*) pObject + 0x4c) = pTarget;
			pFrom = (char*) pSource;
			pTo = (char*) (unsigned long) pTarget;
			for (i = 0; i < cbBuffer; ++i) {
				pTo[i] = pFrom[i];
			}
		}
		*(int*) ((char*) pObject + 0x58) = 1;
	}
	else {
		if (*(unsigned int*) ((char*) pObject + 0x38) == 0) {
			pTarget =
				((MogLoadResourceArchive*) g_pMainResourceArchive)->AllocateResourceDataBufferWithEviction(cbBuffer);
			*(unsigned int*) ((char*) pObject + 0x38) = pTarget;
			pFrom = (char*) pSource;
			pTo = (char*) (unsigned long) pTarget;
			for (i = 0; i < cbBuffer; ++i) {
				pTo[i] = pFrom[i];
			}
		}
		*(int*) ((char*) pObject + 0x5c) = 1;
	}

	if (*(int*) ((char*) pObject + 0x10) == 0 && *(int*) ((char*) pObject + 0x5c) != 0 &&
		*(int*) ((char*) pObject + 0x58) != 0) {
		void** pVtable;
		int cEntries;
		int i;
		int DescriptorCursor;
		int DataCursor;

		pVtable = *(void***) pObject;
		cEntries = *(unsigned int*) ((char*) pObject + 0x6c) / ((unsigned int*) *(void**) ((char*) pObject + 0x48))[1];
		DataCursor = *(int*) ((char*) pObject + 0x38);
		if (*(int*) ((char*) pObject + 0x54) == 0) {
			DescriptorCursor = *(int*) ((char*) pObject + 0x4c);
			*(int*) ((char*) pObject + 0x64) =
				((int(LEMBALL_FASTCALL*)(void*, int)) pVtable[0x12])(pObject, 0) * cEntries;
			if (*(int*) ((char*) pObject + 0x64) == 0) {
				*(int*) ((char*) pObject + 0x64) = -1;
			}
			for (i = 0; i < cEntries; ++i) {
				((int(LEMBALL_FASTCALL*)(void*, int, int, int*, int*))
					 pVtable[0x13])(pObject, 0, i, &DescriptorCursor, &DataCursor);
			}
			((MogLoadResourceArchive*) g_pResourceArchive)
				->FreeResourceObjectDataBuffer(*(unsigned int*) ((char*) pObject + 0x4c), 1);
			*(int*) ((char*) pObject + 0x4c) = 0;
			*(int*) ((char*) pObject + 0x54) = 1;
		}
		*(int*) ((char*) pObject + 0x10) = 1;
		((void(LEMBALL_FASTCALL*)(void*, int)) pVtable[0x0b])(pObject, 0);
		**(int**) ((char*) pObject + 0x48) = -1;
	}
}

// FUNCTION: LEMBALL 0x0045d430
void LEMBALL_FASTCALL LoadListEntryArraysFromArchive(void* pObject)
{
	typedef int(LEMBALL_FASTCALL * NoArgIntProc)(void*, int);
	typedef void(LEMBALL_FASTCALL * OneArgProc)(void*, int, unsigned int);
	typedef void(LEMBALL_FASTCALL * CopyDataProc)(void*, int, unsigned int, unsigned int*, unsigned int);
	int* pList;
	void** pVtable;
	int aArchiveRange[2];
	unsigned int* pBuffer;

	pList = (int*) pObject;
	if (pList[4] == 0) {
		pVtable = *(void***) pList;
		if (((NoArgIntProc) pVtable[3])(pList, 0) == 0) {
			if (*(int*) pList[0x12] != -1 && pList[0x1b] != *(int*) pList[0x12]) {
				return;
			}
			if (pList[0x15] == 0) {
				((OneArgProc) pVtable[0x11])(pList,
											 0,
											 (unsigned int) pList[0x1b] / (unsigned int) ((int*) pList[0x12])[1]);
				aArchiveRange[0] = pList[0xb];
				aArchiveRange[1] = pList[0x1c];
				pBuffer = (unsigned int*) (pList + 0x13);
				if (((MogLoadResourceArchive*) g_pResourceArchive)
						->LoadResourceArchiveEntryDataIntoBuffer(aArchiveRange, pBuffer, pList) != 0) {
					((CopyDataProc) pVtable[1])(pList, 0, *pBuffer, pBuffer, (unsigned int) pList[0x1c]);
				}
			}
			aArchiveRange[0] = pList[0xb] + pList[0x1c];
			aArchiveRange[1] = pList[0x1d];
			pBuffer = (unsigned int*) (pList + 0xe);
			if (((MogLoadResourceArchive*) g_pResourceArchive)
					->LoadResourceArchiveEntryDataIntoBuffer(aArchiveRange, pBuffer, pList) != 0) {
				((CopyDataProc) pVtable[1])(pList, 0, *pBuffer, pBuffer, (unsigned int) pList[0x1c]);
			}
		}
	}
	pList[9] = 0;
}

// FUNCTION: LEMBALL 0x0045d4f0
int LEMBALL_FASTCALL EnsureIntZrleListEntriesLoaded(void* pObject)
{
	return ListNeedsAgeIncrement(pObject);
}

// FUNCTION: LEMBALL 0x0045d540
void LEMBALL_FASTCALL ReleaseListEntriesAndStreamBuffer(void* pObject, void* pUnusedEdx, int fReleaseMode)

{
	typedef int(LEMBALL_FASTCALL * StateProc)(void* pObject);
	typedef void(LEMBALL_FASTCALL * ReleaseEntryProc)(void* pObject, int nUnusedEdx, int iEntry, int fReleaseMode);
	void** pVtable;
	unsigned int pBuffer;
	unsigned int cEntries;
	unsigned int i;

	(void) pUnusedEdx;
	pVtable = *(void***) pObject;
	if (*(int*) ((char*) pObject + 0x10) == 0) {
		if (((StateProc) pVtable[5])(pObject) == 0) {
			return;
		}
	}
	else {
		*(int*) ((char*) pObject + 0x10) = 0;
		pBuffer = *(unsigned int*) ((char*) pObject + 0x38);
		((MogLoadResourceArchive*) g_pResourceArchive)->FreeResourceObjectDataBuffer(pBuffer, 1);
		*(unsigned int*) ((char*) pObject + 0x38) = 0;
	}

	cEntries = *(unsigned int*) ((char*) pObject + 0x6c) / *(unsigned int*) (*(int*) ((char*) pObject + 0x48) + 4);
	for (i = 0; i < cEntries; ++i) {
		((ReleaseEntryProc) pVtable[21])(pObject, 0, (int) i, fReleaseMode);
	}
	NoopVfunc12();
}

// FUNCTION: LEMBALL 0x0045d5c0
void LEMBALL_FASTCALL LoadListEntriesFromStreamDescriptors(void* pObject, void* pUnused, int nArgument)
{
	GameListResourceStreamInterface* pResource;
	unsigned int i;

	(void) pUnused;
	pResource = (GameListResourceStreamInterface*) pObject;
	if (pResource->PrepareListEntryStream() == 0) {
		return;
	}

	i = 0;
	if (*(unsigned int*) ((char*) pObject + 0x6c) / *(unsigned int*) (*(int*) ((char*) pObject + 0x48) + 4) == 0) {
		return;
	}

	do {
		pResource->LoadListEntryFromStream(i, nArgument);
		++i;
	} while (i < *(unsigned int*) ((char*) pObject + 0x6c) / *(unsigned int*) (*(int*) ((char*) pObject + 0x48) + 4));
}

// FUNCTION: LEMBALL 0x0045d610
void* LoadZrleOnlyListResource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject =
		(int*) ((MogLoadResourceArchive*) g_pMainResourceArchive)->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x7c);
	if (pResourceObject != 0) {
		*(void**) pResourceObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pResourceObject[6] = 0;
		*(void**) pResourceObject = g_GAME_ListResourceBaseVtable;
		pResourceObject[0x12] = (int) (unsigned long) g_pPrimaryResourceTypeTable;
		pResourceObject[0x17] = 0;
		pResourceObject[0x16] = 0;
		pResourceObject[0x15] = 0;
		pResourceObject[0x18] = 0;
		*(void**) pResourceObject = g_GAME_ZrleOnlyListResourceVtable;
		pResourceObject[0x1e] = 0;
		InitializeResourceObjectFromId(pResourceObject, nResourceId);
		pResourceObject[6] = 1;
		return FinalizeLoadedResourceObjectResult(pResourceObject);
	}
	return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045d6a0
void LEMBALL_FASTCALL AllocateZrleOnlyListEntryArray(void* pObject, int, int cEntries)
{
	int* pAllocation;
	char* pEntry;
	int i;

	pAllocation = (int*) AllocateVSMemBlock(cEntries * 0x54 + 4);
	if (pAllocation == 0) {
		*(void**) ((char*) pObject + 0x78) = 0;
		return;
	}
	*pAllocation = cEntries;
	pEntry = (char*) (pAllocation + 1);
	for (i = 0; i < cEntries; ++i, pEntry += 0x54) {
		InitializeZrleOnlyListEntry(pEntry);
	}
	*(void**) ((char*) pObject + 0x78) = pAllocation + 1;
}

// FUNCTION: LEMBALL 0x0045d700
void LEMBALL_FASTCALL DispatchZrleOnlyListEntryMethod(void* pObject, int, int iEntry, int nArgument)
{
	char* pEntry;

	pEntry = (char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x54;
	((void(LEMBALL_FASTCALL*)(void*, int, int))(*(void***) pEntry)[9])(pEntry, 0, nArgument);
}

// FUNCTION: LEMBALL 0x0045d720
int LEMBALL_FASTCALL HasZrleOnlyListEntryDescriptor(void* pObject, int, int iEntry)
{
	char* pEntry;
	void** pVtable;

	pEntry = (char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x54;
	pVtable = *(void***) pEntry;
	if (((int(LEMBALL_FASTCALL*)(void*)) pVtable[3])(pEntry) != 0) {
		return 1;
	}
	return ((int(LEMBALL_FASTCALL*)(void*)) pVtable[3])(pEntry);
}

// FUNCTION: LEMBALL 0x0045d7b0
void* ConstructIntZrleListResourceFromId(void* pObject, int nResourceId)
{
	int* pListResource;

	pListResource = (int*) pObject;
	*(void**) pListResource = g_MOGLOAD_CachedResourceObjectBaseVtable;
	pListResource[0x12] = (int) (unsigned long) g_pSecondaryResourceTypeTable;
	*(void**) pListResource = g_GAME_ListResourceBaseVtable;
	*(void**) pListResource = g_GAME_IntZrleListResourceVtable;
	pListResource[6] = 0;
	pListResource[0x17] = 0;
	pListResource[0x16] = 0;
	pListResource[0x15] = 0;
	pListResource[0x18] = 0;
	pListResource[0x1f] = 0;
	pListResource[0x20] = 0;
	pListResource[0x1e] = 0;
	InitializeResourceObjectFromId(pListResource, nResourceId);
	pListResource[6] = 1;
	return pObject;
}

// FUNCTION: LEMBALL 0x0045d850
void* LoadListResource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject =
		(int*) ((MogLoadResourceArchive*) g_pMainResourceArchive)->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x84);
	if (pResourceObject != 0) {
		return FinalizeLoadedResourceObjectResult(ConstructIntZrleListResourceFromId(pResourceObject, nResourceId));
	}
	return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045dd90
void* LoadPalResource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject =
		(int*) ((MogLoadResourceArchive*) g_pMainResourceArchive)->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_GAME_PaletteResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x4c);
	if (pResourceObject != 0) {
		*(void**) pResourceObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pResourceObject[6] = 0;
		*(void**) pResourceObject = g_pPaletteResourceVtable;
		InitializeResourceObjectFromId(pResourceObject, nResourceId);
		return FinalizeLoadedResourceObjectResult(pResourceObject);
	}
	return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045de70
void* LoadTwoArrayListResource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject =
		(int*) ((MogLoadResourceArchive*) g_pMainResourceArchive)->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x80);
	if (pResourceObject != 0) {
		return FinalizeLoadedResourceObjectResult(ConstructTwoArrayListResourceFromId(pResourceObject, nResourceId));
	}
	return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045df20
void* ConstructTwoArrayListResourceFromId(void* pObject, int nResourceId)
{
	int* pListResource;

	pListResource = (int*) pObject;
	*(void**) pListResource = g_MOGLOAD_CachedResourceObjectBaseVtable;
	pListResource[0x12] = (int) (unsigned long) g_pTertiaryResourceTypeTable;
	*(void**) pListResource = g_GAME_ListResourceBaseVtable;
	pListResource[6] = 0;
	pListResource[0x17] = 0;
	pListResource[0x16] = 0;
	pListResource[0x15] = 0;
	pListResource[0x18] = 0;
	*(void**) pListResource = g_GAME_TwoArrayListResourceVtable;
	pListResource[0x1e] = 0;
	pListResource[0x1f] = 0;
	InitializeResourceObjectFromId(pListResource, nResourceId);
	pListResource[6] = 0;
	return pObject;
}

// FUNCTION: LEMBALL 0x0045df70
void LEMBALL_FASTCALL DestroyTwoArrayListResource(void* pObject)
{
	void* pChild;

	*(void**) pObject = g_GAME_TwoArrayListResourceVtableStorage;
	pChild = *(void**) ((char*) pObject + 0x78);
	if (pChild != 0) {
		DestroyGRTSResourceAndFree(pChild, 0, 3);
	}
	pChild = *(void**) ((char*) pObject + 0x7c);
	if (pChild != 0) {
		DestroyIntResourceEntryArray(pChild, 0, 3);
	}
	InitializeTypedResourceObjectBaseVtable(pObject);
}

// FUNCTION: LEMBALL 0x0045dfa0
void LEMBALL_FASTCALL AllocateTwoArrayListEntryArrays(void* pObject, int, int cEntries)
{
	int* pAllocation;
	char* pEntry;
	int i;

	pAllocation = (int*) AllocateVSMemBlock(cEntries * 0x48 + 4);
	if (pAllocation != 0) {
		pAllocation[0] = cEntries;
		pEntry = (char*) (pAllocation + 1);
		for (i = 0; i < cEntries; ++i, pEntry += 0x48) {
			InitializeTypedResourceObjectBaseVtable(pEntry);
			*(void**) pEntry = g_pStringResourceVtable;
			*(int*) (pEntry + 0x18) = 0;
			ResetTypedResourceObjectState(pEntry);
		}
		*(void**) ((char*) pObject + 0x78) = pAllocation + 1;
	}
	else {
		*(void**) ((char*) pObject + 0x78) = 0;
	}

	pAllocation = (int*) AllocateVSMemBlock(cEntries * 0x4c + 4);
	if (pAllocation != 0) {
		pAllocation[0] = cEntries;
		pEntry = (char*) (pAllocation + 1);
		for (i = 0; i < cEntries; ++i, pEntry += 0x4c) {
			InitializeTypedResourceObjectBaseVtable(pEntry);
			*(void**) pEntry = g_GAME_IntResourceVtableStorage;
			*(int*) (pEntry + 0x18) = 0;
			ResetTypedResourceObjectState(pEntry);
		}
		*(void**) ((char*) pObject + 0x7c) = pAllocation + 1;
	}
	else {
		*(void**) ((char*) pObject + 0x7c) = 0;
	}
}

// FUNCTION: LEMBALL 0x0045e060
int LEMBALL_FASTCALL
LoadTwoArrayListEntryPairFromBuffers(void* pObject, int, int iEntry, int* pDescriptorCursor, int* pDataCursor)
{
	int fFailed;

	fFailed = ((MogLoadTypedResourceObject*) ((char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x48))
				  ->LoadTypedResourceObjectFromBufferDescriptor(pDescriptorCursor, pDataCursor, pObject) != 0;
	if (!fFailed) {
		fFailed = ((MogLoadTypedResourceObject*) ((char*) *(void**) ((char*) pObject + 0x7c) + iEntry * 0x4c))
					  ->LoadTypedResourceObjectFromBufferDescriptor(pDescriptorCursor, pDataCursor, pObject) != 0;
	}
	return fFailed;
}

// FUNCTION: LEMBALL 0x0045e0c0
int LEMBALL_FASTCALL LoadTwoArrayListEntryPairFromStream(void* pObject, int, int iEntry, int* pDataCursor)
{
	int fFailed;

	fFailed = ((MogLoadTypedResourceObject*) ((char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x48))
				  ->LoadTypedResourceObjectFromStreamDescriptor(pDataCursor, pObject) != 0;
	if (!fFailed) {
		fFailed = ((MogLoadTypedResourceObject*) ((char*) *(void**) ((char*) pObject + 0x7c) + iEntry * 0x4c))
					  ->LoadTypedResourceObjectFromStreamDescriptor(pDataCursor, pObject) != 0;
	}
	return fFailed;
}

// FUNCTION: LEMBALL 0x0045e120
void LEMBALL_FASTCALL ReleaseTwoArrayListEntryPair(void* pObject, int, int iEntry, int fReleaseMode)
{
	ReleaseTypedResourceObjectIfLoaded((char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x48, 0, fReleaseMode);
	ReleaseTypedResourceObjectIfLoaded((char*) *(void**) ((char*) pObject + 0x7c) + iEntry * 0x4c, 0, fReleaseMode);
}

// FUNCTION: LEMBALL 0x0045e210
void* LoadBitmapResource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject =
		(int*) ((MogLoadResourceArchive*) g_pMainResourceArchive)->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_GAME_BitmapResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x54);
	if (pResourceObject != 0) {
		*(void**) pResourceObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pResourceObject[6] = 0;
		*(void**) pResourceObject = g_pDestroyGRTSResourceAndFreeThunk;
		*(unsigned short*) ((char*) pResourceObject + 0x4a) = 0;
		*(unsigned short*) (pResourceObject + 0x12) = 0;
		*(void**) pResourceObject = g_pBitmapResourceVtable;
		InitializeResourceObjectFromId(pResourceObject, nResourceId);
		return FinalizeLoadedResourceObjectResult(pResourceObject);
	}
	return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045e2b0
void* LoadZrleResource(int nResourceId)
{
	int* pResourceObject;

	pResourceObject =
		(int*) ((MogLoadResourceArchive*) g_pMainResourceArchive)->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_GAME_ZrleResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pResourceObject = (int*) AllocateVSMemBlock(0x54);
	if (pResourceObject != 0) {
		*(void**) pResourceObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pResourceObject[6] = 0;
		*(void**) pResourceObject = g_pDestroyGRTSResourceAndFreeThunk;
		*(unsigned short*) ((char*) pResourceObject + 0x4a) = 0;
		*(unsigned short*) (pResourceObject + 0x12) = 0;
		*(unsigned short*) ((char*) pResourceObject + 0x4e) = 0;
		*(unsigned short*) (pResourceObject + 0x13) = 0;
		*(unsigned short*) ((char*) pResourceObject + 0x52) = 0;
		*(unsigned short*) (pResourceObject + 0x14) = 0;
		*(void**) pResourceObject = g_GAME_ZrleResourceVtable;
		InitializeResourceObjectFromId(pResourceObject, nResourceId);
		return FinalizeLoadedResourceObjectResult(pResourceObject);
	}
	return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045e340
void LEMBALL_FASTCALL PopulateZrleResourceDescriptorFields(void* pObject)
{
	unsigned short* pDescriptor;

	pDescriptor = *(unsigned short**) ((char*) pObject + 0x34);
	*(unsigned short*) ((char*) pObject + 0x48) = pDescriptor[0];
	*(unsigned short*) ((char*) pObject + 0x4a) = pDescriptor[1];
	*(unsigned short*) ((char*) pObject + 0x4c) = pDescriptor[2];
	*(unsigned short*) ((char*) pObject + 0x4e) = pDescriptor[3];
	*(unsigned short*) ((char*) pObject + 0x50) = pDescriptor[4];
	*(unsigned short*) ((char*) pObject + 0x52) = pDescriptor[5];
}

// FUNCTION: LEMBALL 0x0045e680
void LEMBALL_FASTCALL SetListResourceTypeList(void* pObject)
{
	*(unsigned int*) ((char*) pObject + 0x40) = g_GAME_ListResourceTypeTag;
	*(unsigned int*) ((char*) pObject + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045e690
int LEMBALL_FASTCALL GetListEntryArrayCursor(void* pObject)
{
	return *(int*) ((char*) pObject + 0x74);
}

// FUNCTION: LEMBALL 0x0045e6a0
int LEMBALL_FASTCALL ListNeedsAgeIncrement(void* pObject)
{
	return *(int*) ((char*) pObject + 0x60) == *(int*) ((char*) pObject + 0x64);
}

// FUNCTION: LEMBALL 0x0045e6b0
int LEMBALL_FASTCALL ListField60Nonzero(void* pObject)
{
	return *(int*) ((char*) pObject + 0x60) != 0;
}

// FUNCTION: LEMBALL 0x0045e6c0
int LEMBALL_FASTCALL ListField68Nonzero(void* pObject)
{
	return *(int*) ((char*) pObject + 0x68) != 0;
}

// FUNCTION: LEMBALL 0x0045e700
void* LEMBALL_FASTCALL DeleteListResourceBase(void* pObject, int nUnused, int fDelete)

{
	(void) nUnused;
	InitializeTypedResourceObjectBaseVtable(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x0045e720
int LEMBALL_FASTCALL ListIsFirstEntryLoaded(void* pResource)
{
	int fLoaded;

	fLoaded = 0;
	if (*(int*) ((char*) *(void**) ((char*) pResource + 0x78) + 0x18) != 0) {
		fLoaded = 1;
	}
	return fLoaded;
}

// FUNCTION: LEMBALL 0x0045e730
void LEMBALL_FASTCALL LoadZrleOnlyListEntryFromStream(void* pObject, int, int iEntry, int* pDescriptor)
{
	((MogLoadTypedResourceObject*) ((char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x54))
		->LoadTypedResourceObjectFromStreamDescriptor(pDescriptor, pObject);
}

// FUNCTION: LEMBALL 0x0045e750
void LEMBALL_FASTCALL
LoadZrleOnlyListEntryFromBuffer(void* pObject, int, int iEntry, int* pDescriptorCursor, int* pDataCursor)
{
	((MogLoadTypedResourceObject*) ((char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x54))
		->LoadTypedResourceObjectFromBufferDescriptor(pDescriptorCursor, pDataCursor, pObject);
}

// FUNCTION: LEMBALL 0x0045e780
void LEMBALL_FASTCALL ReleaseZrleOnlyListEntry(void* pObject, int, int iEntry, int fReleaseMode)
{
	ReleaseTypedResourceObjectIfLoaded((char*) *(void**) ((char*) pObject + 0x78) + iEntry * 0x54, 0, fReleaseMode);
}

// FUNCTION: LEMBALL 0x0045e7a0
void* LEMBALL_FASTCALL DestroyZrleOnlyListResource(void* pObject, int, int fDelete)

{
	GameListEntryArrayDeleteInterface* pEntryArray;

	*(void**) pObject = g_GAME_ZrleOnlyListResourceVtableStorage;
	pEntryArray = *(GameListEntryArrayDeleteInterface**) ((char*) pObject + 0x78);
	if (pEntryArray != 0) {
		pEntryArray->Delete(3);
	}
	InitializeTypedResourceObjectBaseVtable(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x0045e7e0
void InitializeZrleOnlyListEntry(void* pEntry)
{
	char* pBytes;

	pBytes = (char*) pEntry;
	*(short*) (pBytes + 0x4a) = 0;
	InitializeTypedResourceObjectBaseVtable(pEntry);
	*(short*) (pBytes + 0x48) = 0;
	*(int*) (pBytes + 0x18) = 0;
	*(short*) (pBytes + 0x4e) = 0;
	*(void**) pEntry = g_GAME_ZrleResourceVtableStorage;
	*(short*) (pBytes + 0x4c) = 0;
	*(short*) (pBytes + 0x52) = 0;
	*(short*) (pBytes + 0x50) = 0;
	ResetTypedResourceObjectState(pEntry);
}

// FUNCTION: LEMBALL 0x0045e840
void LEMBALL_FASTCALL SetupZrleResourceType(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x5a524c45;
	*(int*) ((char*) pResource + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045e850
void* LEMBALL_FASTCALL DestroyZrleResourceEntryArray(void* pObject, int nUnused, unsigned int fDelete)
{
	(void) nUnused;
	int cEntries;
	char* pEntry;

	if ((fDelete & 2) == 0) {
		InitializeTypedResourceObjectBaseVtable(pObject);
		if ((fDelete & 1) != 0) {
			FreeVSMemBlock(pObject);
		}
		return pObject;
	}

	cEntries = *((int*) pObject - 1);
	pEntry = (char*) pObject + cEntries * 0x54;
	while (cEntries-- > 0) {
		pEntry -= 0x54;
		InitializeTypedResourceObjectBaseVtable(pEntry);
	}
	FreeVSMemBlock((char*) pObject - 4);
	return pObject;
}

// FUNCTION: LEMBALL 0x0045e910
void LEMBALL_FASTCALL SetIntResourceTypeTag(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x494e5420;
}

// FUNCTION: LEMBALL 0x0045e920
void LEMBALL_FASTCALL ReadIntResourcePayloadFromStream(void* pResource)
{
	*(int*) ((char*) pResource + 0x48) = **(int**) ((char*) pResource + 0x38);
}

// FUNCTION: LEMBALL 0x0045e930
void* LEMBALL_FASTCALL DestroyIntResourceEntryArray(void* pObject, int nUnused, unsigned int fDelete)
{
	int cEntries;
	char* pEntry;

	(void) nUnused;
	if ((fDelete & 2) == 0) {
		InitializeTypedResourceObjectBaseVtable(pObject);
		if ((fDelete & 1) != 0) {
			FreeVSMemBlock(pObject);
		}
		return pObject;
	}

	cEntries = *((int*) pObject - 1);
	pEntry = (char*) pObject + cEntries * 0x4c;
	while (cEntries-- > 0) {
		pEntry -= 0x4c;
		InitializeTypedResourceObjectBaseVtable(pEntry);
	}
	FreeVSMemBlock((char*) pObject - 4);
	return pObject;
}

// FUNCTION: LEMBALL 0x0045ea30
void LEMBALL_FASTCALL SetPalResourceType(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x50414c20;
	*(int*) ((char*) pResource + 0x3c) = 4;
}

// FUNCTION: LEMBALL 0x0045ea80
void* LEMBALL_FASTCALL DestroyTwoArrayListResourceAndFree(void* pObject, int nUnused, int fDelete)

{
	(void) nUnused;
	DestroyTwoArrayListResource(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x0045eb40
void LEMBALL_FASTCALL SetGamiResourceTypeTag(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x494d4147;
	*(int*) ((char*) pResource + 0x3c) = 0x10;
}

// FUNCTION: LEMBALL 0x0045eb70
void LEMBALL_FASTCALL SetBitmapResourceType(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x42544d50;
	*(int*) ((char*) pResource + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045eba0
void LEMBALL_FASTCALL SetEffResourceTypeTag(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x45464620;
}

// FUNCTION: LEMBALL 0x0045ebd0
void LEMBALL_FASTCALL SetPtchResourceTypeTag(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x50544348;
}

// FUNCTION: LEMBALL 0x0045ec00
void LEMBALL_FASTCALL SetTuneResourceTypeTag(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x54554e45;
}

// FUNCTION: LEMBALL 0x0045ec30
void LEMBALL_FASTCALL SetPresResourceTypeTag(void* pResource)
{
	*(unsigned int*) ((char*) pResource + 0x40) = 0x50524553;
}
