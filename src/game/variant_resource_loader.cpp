#include "../game/game_app.h"
#include "../resource/resource_archive.h"
#include "../engine/memory_arena.h"
#include "../network/safe_vtable.h"

struct GAME_MainGameVariantResourceBundleLoader {
    void **m_pVtable;
    void *m_pStatusIndicatorManager;
    int m_nLoadedResourceCount;
    int m_nTotalResourceCount;
    void **m_ppZrleListResources;
    void *m_pReserved14;
    int m_cZrleListResources;
    void **m_ppListResources;
    void *m_pReserved20;
    int m_cListResources;
    void **m_ppBitmapResources;
    void *m_pReserved2C;
    int m_cBitmapResources;
    void **m_ppPaletteResources;
    int m_cPaletteResources;
    void **m_ppStringResources;
    int m_cStringResources;
    void **m_ppTwoArrayListResources;
    int m_cTwoArrayListResources;
};

static const unsigned int g_GAME_ListResourceTypeTag = 0x4c495354;
static const unsigned int g_GAME_BitmapResourceTypeTag = 0x42544d50;
static const unsigned int g_GAME_PaletteResourceTypeTag = 0x50414c20;
static const unsigned int g_GAME_ZrleResourceTypeTag = 0x5a524c45;

extern void InitializeTypedResourceObjectBaseVtable(void *pObject);
extern void CopyBufferIntoTypedResourceObjectAndParse(void *pObject, unsigned int *pSource,
                                                       unsigned int nUnused, unsigned int cbBuffer);
extern void ParseIntResourceDescriptor(void);
extern void UnloadTypedResourceObject(void *pObject, int fReleaseMode);
extern void EnsureTypedResourceObjectLoaded(void *pObject);
extern int GetField14NeedsAgeIncrement(void *pObject);
extern int GetField1CVfunc04(void *pObject);
extern int GetField14Vfunc05(void *pObject);
extern int ReturnZeroVfunc06(void);
extern void NoopVfunc09(void);
extern void *GetEffResourceDataPointer(void *pObject);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern void ClearResourceTypeTag(void *pObject);
extern int GetField28GetMemorySize(void *pObject);

// FUNCTION: LEMBALL 0x0045E340
void PopulateZrleResourceDescriptorFields(void *pObject) {
    unsigned short *pDescriptor;

    pDescriptor = *(unsigned short **)((char *)pObject + 0x34);
    *(unsigned short *)((char *)pObject + 0x48) = pDescriptor[0];
    *(unsigned short *)((char *)pObject + 0x4a) = pDescriptor[1];
    *(unsigned short *)((char *)pObject + 0x4c) = pDescriptor[2];
    *(unsigned short *)((char *)pObject + 0x4e) = pDescriptor[3];
    *(unsigned short *)((char *)pObject + 0x50) = pDescriptor[4];
    *(unsigned short *)((char *)pObject + 0x52) = pDescriptor[5];
}

// FUNCTION: LEMBALL 0x0045E850
void *DestroyZrleResourceEntryArray(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        InitializeTypedResourceObjectBaseVtable(pObject);
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x54;
    while (cEntries-- > 0) {
        pEntry -= 0x54;
        InitializeTypedResourceObjectBaseVtable(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

static void *g_GAME_ZrleResourceVtableStorage[15] = {
    (void *)DestroyZrleResourceEntryArray,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)PopulateZrleResourceDescriptorFields,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)ClearResourceTypeTag,
    (void *)GetField28GetMemorySize,
};

extern void *g_pDestroyGRTSResourceAndFreeThunk;
extern void *g_pCachedResourceObjectBaseDeleteVtable;
extern void *g_pBitmapResourceVtable;
extern void *g_pPaletteResourceVtable;

// LIST resource objects use the same typed-resource prefix, then keep their
// descriptor-derived arrays at +0x38/+0x4c and list state at +0x60..+0x74.
static void *DeleteListResourceBase(void *pObject, int fDelete);
static void DestroyTwoArrayListResource(void *pObject);
static void *DestroyTwoArrayListResourceAndFree(void *pObject, int fDelete);
static void CopyBuffersIntoTwoArrayListResourceAndParse(void *pObject,
                                                         unsigned int *pSource,
                                                         int pRelatedObject,
                                                         unsigned int cbBuffer);
static void ParseListResourceDescriptor(void *pObject);
static int ListNeedsAgeIncrement(void *pObject);
static int ListField68Nonzero(void *pObject);
static int ListField60Nonzero(void *pObject);
static int EnsureIntZrleListEntriesLoaded(void *pObject);
static void LoadListEntryArraysFromArchive(void *pObject);
static void ReleaseListEntriesAndStreamBuffer(void *pObject, int fReleaseMode);
static void LoadListEntriesFromStreamDescriptors(void *pObject, int nArgument);
static void SetListResourceTypeList(void *pObject);
static int GetListEntryArrayCursor(void *pObject);
static void *g_GAME_ListResourceVtableStorage[15] = {
    (void *)DeleteListResourceBase,
    (void *)CopyBuffersIntoTwoArrayListResourceAndParse,
    (void *)ParseListResourceDescriptor,
    (void *)ListNeedsAgeIncrement,
    (void *)ListField68Nonzero,
    (void *)ListField60Nonzero,
    (void *)EnsureIntZrleListEntriesLoaded,
    (void *)LoadListEntryArraysFromArchive,
    (void *)ReleaseListEntriesAndStreamBuffer,
    (void *)LoadListEntriesFromStreamDescriptors,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetListResourceTypeList,
    (void *)GetListEntryArrayCursor,
};
static void *g_GAME_TwoArrayListResourceVtableStorage[15] = {
    (void *)DestroyTwoArrayListResourceAndFree,
    (void *)CopyBuffersIntoTwoArrayListResourceAndParse,
    (void *)ParseListResourceDescriptor,
    (void *)ListNeedsAgeIncrement,
    (void *)ListField68Nonzero,
    (void *)ListField60Nonzero,
    (void *)EnsureIntZrleListEntriesLoaded,
    (void *)LoadListEntryArraysFromArchive,
    (void *)ReleaseListEntriesAndStreamBuffer,
    (void *)LoadListEntriesFromStreamDescriptors,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetListResourceTypeList,
    (void *)GetListEntryArrayCursor,
};
static void *g_GAME_ListResourceBaseVtable = g_GAME_ListResourceVtableStorage;
static void *g_GAME_ZrleOnlyListResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void *g_GAME_IntZrleListResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void *g_GAME_TypedBitmapResourceDeleteVtable = g_pDestroyGRTSResourceAndFreeThunk;
static void *g_GAME_ZrleResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void *g_GAME_BitmapResourceVtable = g_pBitmapResourceVtable;
static void *g_GAME_PaletteResourceVtable = g_pPaletteResourceVtable;
static void *g_GAME_TwoArrayListResourceVtable = g_GAME_TwoArrayListResourceVtableStorage;
static int g_GAME_ZrleListResourceTypeToken = 0;
static int g_GAME_IntZrleListResourceTypeToken = 0;
static int g_GAME_TwoArrayListResourceTypeToken = 0;

extern void *g_pMainResourceArchive;
extern void InitializeResourceObjectFromId(void *pObject, int nResourceId);
extern void *FinalizeLoadedResourceObjectResult(void *pObject);
extern unsigned int AllocateResourceDataBufferWithEviction(void *pArchive, unsigned int cbBuffer);
extern void FreeResourceObjectDataBuffer(unsigned int pBuffer);

// FUNCTION: LEMBALL 0x0045E700
static void *DeleteListResourceBase(void *pObject, int fDelete) {
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EA80
static void *DestroyTwoArrayListResourceAndFree(void *pObject, int fDelete) {
    DestroyTwoArrayListResource(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045DF70
static void DestroyTwoArrayListResource(void *pObject) {
    void *pChild;

    *(void **)pObject = g_GAME_TwoArrayListResourceVtableStorage;
    pChild = *(void **)((char *)pObject + 0x78);
    if (pChild != 0) {
        ((void (*)(void *, int))*(void **)pChild)(pChild, 3);
    }
    pChild = *(void **)((char *)pObject + 0x7c);
    if (pChild != 0) {
        ((void (*)(void *, int))*(void **)pChild)(pChild, 3);
    }
    InitializeTypedResourceObjectBaseVtable(pObject);
}

// FUNCTION: LEMBALL 0x0045D290
static void ParseListResourceDescriptor(void *pObject) {
    unsigned int *pDescriptor;

    pDescriptor = *(unsigned int **)((char *)pObject + 0x34);
    *(unsigned int *)((char *)pObject + 0x6c) = pDescriptor[0];
    *(unsigned int *)((char *)pObject + 0x70) = pDescriptor[1];
    *(unsigned int *)((char *)pObject + 0x64) = 0xffffffff;
    *(unsigned int *)((char *)pObject + 0x74) = pDescriptor[2];
}

// FUNCTION: LEMBALL 0x0045E6A0
static int ListNeedsAgeIncrement(void *pObject) {
    return *(int *)((char *)pObject + 0x60) == *(int *)((char *)pObject + 0x64);
}

// FUNCTION: LEMBALL 0x0045E6C0
static int ListField68Nonzero(void *pObject) {
    return *(int *)((char *)pObject + 0x68) != 0;
}

// FUNCTION: LEMBALL 0x0045E6B0
static int ListField60Nonzero(void *pObject) {
    return *(int *)((char *)pObject + 0x60) != 0;
}

// FUNCTION: LEMBALL 0x0045E690
static int GetListEntryArrayCursor(void *pObject) {
    return *(int *)((char *)pObject + 0x74);
}

// FUNCTION: LEMBALL 0x0045E680
static void SetListResourceTypeList(void *pObject) {
    *(unsigned int *)((char *)pObject + 0x40) = g_GAME_ListResourceTypeTag;
    *(unsigned int *)((char *)pObject + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045D2B0
static void CopyBuffersIntoTwoArrayListResourceAndParse(void *pObject,
                                                         unsigned int *pSource,
                                                         int pRelatedObject,
                                                         unsigned int cbBuffer) {
    unsigned int pTarget;
    unsigned int i;
    char *pFrom;
    char *pTo;

    if ((int)((char *)pObject - (char *)pRelatedObject) == -0x4c) {
        if (*(unsigned int *)((char *)pObject + 0x4c) == 0) {
            pTarget = AllocateResourceDataBufferWithEviction(g_pMainResourceArchive, cbBuffer);
            *(unsigned int *)((char *)pObject + 0x4c) = pTarget;
            pFrom = (char *)pSource;
            pTo = (char *)(unsigned long)pTarget;
            for (i = 0; i < cbBuffer; ++i) {
                pTo[i] = pFrom[i];
            }
        }
        *(int *)((char *)pObject + 0x58) = 1;
    } else {
        if (*(unsigned int *)((char *)pObject + 0x38) == 0) {
            pTarget = AllocateResourceDataBufferWithEviction(g_pMainResourceArchive, cbBuffer);
            *(unsigned int *)((char *)pObject + 0x38) = pTarget;
            pFrom = (char *)pSource;
            pTo = (char *)(unsigned long)pTarget;
            for (i = 0; i < cbBuffer; ++i) {
                pTo[i] = pFrom[i];
            }
        }
        *(int *)((char *)pObject + 0x5c) = 1;
    }
}

// FUNCTION: LEMBALL 0x0045D4F0
static int EnsureIntZrleListEntriesLoaded(void *pObject) {
    return ListNeedsAgeIncrement(pObject);
}

// FUNCTION: LEMBALL 0x0045D430
static void LoadListEntryArraysFromArchive(void *pObject) {
    *(int *)((char *)pObject + 0x24) = 0;
}

// FUNCTION: LEMBALL 0x0045D540
static void ReleaseListEntriesAndStreamBuffer(void *pObject, int fReleaseMode) {
    unsigned int pBuffer;

    pBuffer = *(unsigned int *)((char *)pObject + 0x38);
    if (pBuffer != 0) {
        FreeResourceObjectDataBuffer(pBuffer);
        *(unsigned int *)((char *)pObject + 0x38) = 0;
    }
    *(int *)((char *)pObject + 0x10) = 0;
    (void)fReleaseMode;
}

// FUNCTION: LEMBALL 0x0045D5C0
static void LoadListEntriesFromStreamDescriptors(void *pObject, int nArgument) {
    (void)pObject;
    (void)nArgument;
}

// FUNCTION: LEMBALL 0x0045C7B0
void *ConstructIntZrleListResourceFromId(void *pObject, int nResourceId) {
    int *pListResource;

    pListResource = (int *)pObject;
    *(void **)pListResource = &g_pCachedResourceObjectBaseDeleteVtable;
    pListResource[0x12] = g_GAME_IntZrleListResourceTypeToken;
    *(void **)pListResource = g_GAME_ListResourceBaseVtable;
    *(void **)pListResource = g_GAME_IntZrleListResourceVtable;
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

void *ConstructTwoArrayListResourceFromId(void *pObject, int nResourceId) {
    int *pListResource;

    pListResource = (int *)pObject;
    *(void **)pListResource = &g_pCachedResourceObjectBaseDeleteVtable;
    pListResource[0x12] = g_GAME_TwoArrayListResourceTypeToken;
    *(void **)pListResource = g_GAME_ListResourceBaseVtable;
    pListResource[6] = 0;
    pListResource[0x17] = 0;
    pListResource[0x16] = 0;
    pListResource[0x15] = 0;
    pListResource[0x18] = 0;
    *(void **)pListResource = g_GAME_TwoArrayListResourceVtable;
    pListResource[0x1e] = 0;
    pListResource[0x1f] = 0;
    InitializeResourceObjectFromId(pListResource, nResourceId);
    pListResource[6] = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0045C610
void *LoadZrleOnlyListResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x7c);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = &g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_GAME_ListResourceBaseVtable;
        pResourceObject[0x12] = g_GAME_ZrleListResourceTypeToken;
        pResourceObject[0x17] = 0;
        pResourceObject[0x16] = 0;
        pResourceObject[0x15] = 0;
        pResourceObject[0x18] = 0;
        *(void **)pResourceObject = g_GAME_ZrleOnlyListResourceVtable;
        pResourceObject[0x1e] = 0;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        pResourceObject[6] = 1;
        return FinalizeLoadedResourceObjectResult(pResourceObject);
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045C850
void *LoadListResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x84);
    if (pResourceObject != 0) {
        return FinalizeLoadedResourceObjectResult(ConstructIntZrleListResourceFromId(pResourceObject, nResourceId));
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045CE70
void *LoadTwoArrayListResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x80);
    if (pResourceObject != 0) {
        return FinalizeLoadedResourceObjectResult(ConstructTwoArrayListResourceFromId(pResourceObject, nResourceId));
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045D210
void *LoadBitmapResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_BitmapResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x54);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = &g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_GAME_TypedBitmapResourceDeleteVtable;
        *(unsigned short *)((char *)pResourceObject + 0x4a) = 0;
        *(unsigned short *)(pResourceObject + 0x12) = 0;
        *(void **)pResourceObject = g_GAME_BitmapResourceVtable;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        return FinalizeLoadedResourceObjectResult(pResourceObject);
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045E2B0
void *LoadZrleResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_ZrleResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x54);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = &g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_GAME_TypedBitmapResourceDeleteVtable;
        *(unsigned short *)((char *)pResourceObject + 0x4a) = 0;
        *(unsigned short *)(pResourceObject + 0x12) = 0;
        *(unsigned short *)((char *)pResourceObject + 0x4e) = 0;
        *(unsigned short *)(pResourceObject + 0x13) = 0;
        *(unsigned short *)((char *)pResourceObject + 0x52) = 0;
        *(unsigned short *)(pResourceObject + 0x14) = 0;
        *(void **)pResourceObject = g_GAME_ZrleResourceVtable;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        return FinalizeLoadedResourceObjectResult(pResourceObject);
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045CD90
void *LoadPalResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_PaletteResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x4c);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = &g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_GAME_PaletteResourceVtable;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        return FinalizeLoadedResourceObjectResult(pResourceObject);
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x00446DB0
void LoadMainGameVariantZrleListResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GAME_MainGameVariantResourceBundleLoader *)pBundle;
    ((void (*)())**(void ***)pBundle)();
    pResourceBundle->m_ppZrleListResources[pResourceBundle->m_cZrleListResources] = LoadZrleOnlyListResource(nResourceId);
    ++pResourceBundle->m_cZrleListResources;
}

// FUNCTION: LEMBALL 0x00446E30
void LoadMainGameVariantListResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GAME_MainGameVariantResourceBundleLoader *)pBundle;
    ((void (*)())**(void ***)pBundle)();
    pResourceBundle->m_ppListResources[pResourceBundle->m_cListResources] = LoadListResource(nResourceId);
    ++pResourceBundle->m_cListResources;
}

// FUNCTION: LEMBALL 0x00446EB0
void LoadMainGameVariantBitmapResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GAME_MainGameVariantResourceBundleLoader *)pBundle;
    ((void (*)())**(void ***)pBundle)();
    pResourceBundle->m_ppBitmapResources[pResourceBundle->m_cBitmapResources] = LoadBitmapResource(nResourceId);
    ++pResourceBundle->m_cBitmapResources;
}

// FUNCTION: LEMBALL 0x00446F30
void LoadMainGameVariantPaletteResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GAME_MainGameVariantResourceBundleLoader *)pBundle;
    ((void (*)())**(void ***)pBundle)();
    pResourceBundle->m_ppPaletteResources[pResourceBundle->m_cPaletteResources] = LoadPalResource(nResourceId);
    ++pResourceBundle->m_cPaletteResources;
}

// FUNCTION: LEMBALL 0x00446FB0
void LoadMainGameVariantStringResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GAME_MainGameVariantResourceBundleLoader *)pBundle;
    ((void (*)())**(void ***)pBundle)();
    pResourceBundle->m_ppStringResources[pResourceBundle->m_cStringResources] = LoadStringResource(nResourceId);
    ++pResourceBundle->m_cStringResources;
}

// FUNCTION: LEMBALL 0x00448030
void LoadMainGameVariantTwoArrayListResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GAME_MainGameVariantResourceBundleLoader *)pBundle;
    ((void (*)())**(void ***)pBundle)();
    pResourceBundle->m_ppTwoArrayListResources[pResourceBundle->m_cTwoArrayListResources] = LoadTwoArrayListResource(nResourceId);
    ++pResourceBundle->m_cTwoArrayListResources;
}
#include "../network/safe_vtable.h"
