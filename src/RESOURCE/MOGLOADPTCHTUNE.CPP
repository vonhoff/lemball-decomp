#include "ENGINE/CORE/VSMEM.H"
#include "RESOURCE/MOGLOAD.H"

extern void* g_pResourceArchive;
extern void* g_MOGLOAD_CachedResourceObjectBaseVtable[15];

extern void LEMBALL_FASTCALL
CopyBufferIntoTypedResourceObjectAndParse(void*, void*, unsigned int*, unsigned int, unsigned int);
extern void ParseIntResourceDescriptor(void);
extern int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void*);
extern int LEMBALL_FASTCALL GetField1CVfunc04(void*);
extern int LEMBALL_FASTCALL GetField14Vfunc05(void*);
extern int ReturnZeroVfunc06(void);
extern void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void*);
extern void LEMBALL_FASTCALL UnloadTypedResourceObject(void*, void*, int);
extern void LEMBALL_FASTCALL NoopVfunc09(void*, void*, int);
extern void* LEMBALL_FASTCALL GetEffResourceDataPointer(void*);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern int LEMBALL_FASTCALL GetField28GetMemorySize(void*);
extern void LEMBALL_FASTCALL SetPtchResourceTypeTag(void*);
extern void LEMBALL_FASTCALL SetTuneResourceTypeTag(void*);
extern void* LEMBALL_FASTCALL DestroyPtchResourceAuto(void*, void*, BYTE);
extern void* LEMBALL_FASTCALL DestroyTuneResourceAuto(void*, void*, BYTE);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void*);
extern void __fastcall InitializeResourceObjectFromId(void*, void*, int);
extern void* __fastcall FinalizeLoadedResourceObjectResult(void*, void*);

// GLOBAL: LEMBALL 0x00498da0
static void* g_MOGLOAD_PtchResourceObjectVtable[15] = {
	(void*) DestroyPtchResourceAuto,
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
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetPtchResourceTypeTag,
	(void*) GetField28GetMemorySize,
};

// GLOBAL: LEMBALL 0x00498de0
static void* g_MOGLOAD_TuneResourceObjectVtable[15] = {
	(void*) DestroyTuneResourceAuto,
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
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetTuneResourceTypeTag,
	(void*) GetField28GetMemorySize,
};

inline static void* LoadTypedSoundResource(int nResourceId, unsigned int uTypeTag, void** pVtable)
{
	MogLoadStringResourceObject* pResourceObject;

	pResourceObject = (MogLoadStringResourceObject*) ((MogLoadResourceArchive*) g_pResourceArchive)
						  ->FindCachedResourceObjectById(nResourceId);
	if (pResourceObject == 0) {
		pResourceObject = (MogLoadStringResourceObject*) AllocateVSMemBlock(0x48);
		if (pResourceObject != 0) {
			pResourceObject->m_pVtable = g_MOGLOAD_CachedResourceObjectBaseVtable;
			pResourceObject->m_nReserved18 = 0;
			pResourceObject->m_pVtable = pVtable;
			InitializeResourceObjectFromId(pResourceObject, 0, nResourceId);
			return FinalizeLoadedResourceObjectResult(pResourceObject, 0);
		}
		return FinalizeLoadedResourceObjectResult(0, 0);
	}

	if (pResourceObject->m_uTypeTag != uTypeTag) {
		ReleaseTypedResourceObjectReference(pResourceObject);
		pResourceObject = 0;
	}
	return pResourceObject;
}

// FUNCTION: LEMBALL 0x0045e3f0
void* LoadPtchResource(int nResourceId)
{
	return LoadTypedSoundResource(nResourceId, 0x50544348, g_MOGLOAD_PtchResourceObjectVtable);
}

// FUNCTION: LEMBALL 0x0045e460
void* LoadTuneResource(int nResourceId)
{
	return LoadTypedSoundResource(nResourceId, 0x54554e45, g_MOGLOAD_TuneResourceObjectVtable);
}
