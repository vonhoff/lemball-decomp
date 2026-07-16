#include "GAME.H"
#include "RESOURCE/MOGLOAD.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "RESOURCE/VARLOAD.H"

extern void LEMBALL_FASTCALL CopyBufferIntoTypedResourceObjectAndParse(
    void *pObject,
    void *pUnusedEdx,
    unsigned int *pSource,
    unsigned int nUnused,
    unsigned int cbBuffer);

// FUNCTION: LEMBALL 0x0045E720
int LEMBALL_FASTCALL ListIsFirstEntryLoaded(void *pResource) {
    int fLoaded;

    fLoaded = 0;
    if (*(int *)((char *)*(void **)((char *)pResource + 0x78) + 0x18) != 0) {
        fLoaded = 1;
    }
    return fLoaded;
}

// FUNCTION: LEMBALL 0x0045E840
void LEMBALL_FASTCALL SetupZrleResourceType(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x5a524c45;
    *(int *)((char *)pResource + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045E910
void LEMBALL_FASTCALL SetIntResourceTypeTag(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x494e5420;
}

// FUNCTION: LEMBALL 0x0045E920
void LEMBALL_FASTCALL ReadIntResourcePayloadFromStream(void *pResource) {
    *(int *)((char *)pResource + 0x48) = **(int **)((char *)pResource + 0x38);
}

// FUNCTION: LEMBALL 0x0045EA30
void LEMBALL_FASTCALL SetPalResourceType(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x50414c20;
    *(int *)((char *)pResource + 0x3c) = 4;
}

// FUNCTION: LEMBALL 0x0045EB40
void LEMBALL_FASTCALL SetGamiResourceTypeTag(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x494d4147;
    *(int *)((char *)pResource + 0x3c) = 0x10;
}

// FUNCTION: LEMBALL 0x0045EB70
void LEMBALL_FASTCALL SetBitmapResourceType(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x42544d50;
    *(int *)((char *)pResource + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045EBA0
void LEMBALL_FASTCALL SetEffResourceTypeTag(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x45464620;
}

// FUNCTION: LEMBALL 0x0045EBD0
void LEMBALL_FASTCALL SetPtchResourceTypeTag(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x50544348;
}

// FUNCTION: LEMBALL 0x0045EC00
void LEMBALL_FASTCALL SetTuneResourceTypeTag(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x54554e45;
}

// FUNCTION: LEMBALL 0x0045EC30
void LEMBALL_FASTCALL SetPresResourceTypeTag(void *pResource) {
    *(unsigned int *)((char *)pResource + 0x40) = 0x50524553;
}

static const unsigned int g_GAME_ListResourceTypeTag = 0x4c495354;
static const unsigned int g_GAME_BitmapResourceTypeTag = 0x42544d50;
static const unsigned int g_GAME_PaletteResourceTypeTag = 0x50414c20;
static const unsigned int g_GAME_ZrleResourceTypeTag = 0x5a524c45;

extern void LEMBALL_FASTCALL InitializeTypedResourceObjectBaseVtable(void *pObject);
extern void ParseIntResourceDescriptor(void);
extern void LEMBALL_FASTCALL UnloadTypedResourceObject(
    void *pObject, void *pUnusedEcx, int fReleaseMode);
extern void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void *pObject);
extern int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void *pObject);
extern int LEMBALL_FASTCALL GetField1CVfunc04(void *pObject);
extern int LEMBALL_FASTCALL GetField14Vfunc05(void *pObject);
extern int ReturnZeroVfunc06(void);
extern void LEMBALL_FASTCALL NoopVfunc09(
    void *pObject, void *pUnusedEdx, int nArgument);
extern void *LEMBALL_FASTCALL GetEffResourceDataPointer(void *pObject);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern void LEMBALL_FASTCALL ClearResourceTypeTag(void *pObject);
extern int LEMBALL_FASTCALL GetField28GetMemorySize(void *pObject);

// FUNCTION: LEMBALL 0x0045E340
void LEMBALL_FASTCALL PopulateZrleResourceDescriptorFields(void *pObject) {
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
void *LEMBALL_FASTCALL DestroyZrleResourceEntryArray(
    void *pObject, int nUnused, unsigned int fDelete) {
    (void)nUnused;
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
    (void *)SetupZrleResourceType,
    (void *)GetField28GetMemorySize,
};

extern void *g_pDestroyGRTSResourceAndFreeThunk;
extern void *g_pCachedResourceObjectBaseDeleteVtable;
extern void *g_pBitmapResourceVtable;
extern void *g_pPaletteResourceVtable;

struct GameVariantResourceBundleLoadInterface {
    virtual void PrepareNextResourceLoad(void) = 0;
};

struct GameListEntryArrayDeleteInterface {
    virtual void *Delete(int fDelete) = 0;
};

// LIST resource objects use the same typed-resource prefix, then keep their
// descriptor-derived arrays at +0x38/+0x4c and list state at +0x60..+0x74.
static void *LEMBALL_FASTCALL DeleteListResourceBase(
    void *pObject, int nUnused, int fDelete);
static void *LEMBALL_FASTCALL DestroyZrleOnlyListResource(
    void *pObject, int nUnused, int fDelete);
static void LEMBALL_FASTCALL DestroyTwoArrayListResource(void *pObject);
static void *LEMBALL_FASTCALL DestroyTwoArrayListResourceAndFree(
    void *pObject, int nUnused, int fDelete);
static void LEMBALL_FASTCALL CopyBuffersIntoTwoArrayListResourceAndParse(
    void *pObject,
    void *pUnusedEdx,
    unsigned int *pSource,
    int pRelatedObject,
    unsigned int cbBuffer);
static void LEMBALL_FASTCALL ParseListResourceDescriptor(void *pObject);
static int LEMBALL_FASTCALL ListNeedsAgeIncrement(void *pObject);
static int LEMBALL_FASTCALL ListField68Nonzero(void *pObject);
static int LEMBALL_FASTCALL ListField60Nonzero(void *pObject);
static int LEMBALL_FASTCALL EnsureIntZrleListEntriesLoaded(void *pObject);
static void LEMBALL_FASTCALL LoadListEntryArraysFromArchive(void *pObject);
static void LEMBALL_FASTCALL ReleaseListEntriesAndStreamBuffer(
    void *pObject, void *pUnusedEdx, int fReleaseMode);
static void LEMBALL_FASTCALL LoadListEntriesFromStreamDescriptors(
    void *pObject, void *pUnused, int nArgument);
static void LEMBALL_FASTCALL SetListResourceTypeList(void *pObject);
static int LEMBALL_FASTCALL GetListEntryArrayCursor(void *pObject);
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
// GLOBAL: LEMBALL 0x00498A18
static void *g_GAME_ZrleOnlyListResourceVtableStorage[15] = {
    (void *)DestroyZrleOnlyListResource,
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
static void *g_GAME_ZrleOnlyListResourceVtable =
    g_GAME_ZrleOnlyListResourceVtableStorage;
static void *g_GAME_IntZrleListResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void *g_GAME_ZrleResourceVtable = g_GAME_ZrleResourceVtableStorage;
static void *g_GAME_TwoArrayListResourceVtable = g_GAME_TwoArrayListResourceVtableStorage;
static int g_GAME_ZrleListResourceTypeToken = 0;
static int g_GAME_IntZrleListResourceTypeToken = 0;
static int g_GAME_TwoArrayListResourceTypeToken = 0;

extern void *g_pMainResourceArchive;
extern void *g_pResourceArchive;
extern void InitializeResourceObjectFromId(void *pObject, int nResourceId);
extern void *FinalizeLoadedResourceObjectResult(void *pObject);

// FUNCTION: LEMBALL 0x0045E700
static void *LEMBALL_FASTCALL DeleteListResourceBase(
    void *pObject, int nUnused, int fDelete)
{
    (void)nUnused;
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045E7A0
static void *LEMBALL_FASTCALL DestroyZrleOnlyListResource(
    void *pObject, int, int fDelete)
{
    GameListEntryArrayDeleteInterface *pEntryArray;

    *(void **)pObject = g_GAME_ZrleOnlyListResourceVtableStorage;
    pEntryArray = *(GameListEntryArrayDeleteInterface **)
        ((char *)pObject + 0x78);
    if (pEntryArray != 0) {
        pEntryArray->Delete(3);
    }
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EA80
static void *LEMBALL_FASTCALL DestroyTwoArrayListResourceAndFree(
    void *pObject, int nUnused, int fDelete)
{
    (void)nUnused;
    DestroyTwoArrayListResource(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045DF70
static void LEMBALL_FASTCALL DestroyTwoArrayListResource(void *pObject) {
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
static void LEMBALL_FASTCALL ParseListResourceDescriptor(void *pObject) {
    unsigned int *pDescriptor;

    pDescriptor = *(unsigned int **)((char *)pObject + 0x34);
    *(unsigned int *)((char *)pObject + 0x6c) = pDescriptor[0];
    *(unsigned int *)((char *)pObject + 0x70) = pDescriptor[1];
    *(unsigned int *)((char *)pObject + 0x74) = pDescriptor[2];
    *(unsigned int *)((char *)pObject + 0x64) = 0xffffffff;
}

// FUNCTION: LEMBALL 0x0045E6A0
static int LEMBALL_FASTCALL ListNeedsAgeIncrement(void *pObject) {
    return *(int *)((char *)pObject + 0x60) == *(int *)((char *)pObject + 0x64);
}

// FUNCTION: LEMBALL 0x0045E6C0
static int LEMBALL_FASTCALL ListField68Nonzero(void *pObject) {
    return *(int *)((char *)pObject + 0x68) != 0;
}

// FUNCTION: LEMBALL 0x0045E6B0
static int LEMBALL_FASTCALL ListField60Nonzero(void *pObject) {
    return *(int *)((char *)pObject + 0x60) != 0;
}

// FUNCTION: LEMBALL 0x0045E690
static int LEMBALL_FASTCALL GetListEntryArrayCursor(void *pObject) {
    return *(int *)((char *)pObject + 0x74);
}

// FUNCTION: LEMBALL 0x0045E680
static void LEMBALL_FASTCALL SetListResourceTypeList(void *pObject) {
    *(unsigned int *)((char *)pObject + 0x40) = g_GAME_ListResourceTypeTag;
    *(unsigned int *)((char *)pObject + 0x3c) = 0x0c;
}

// FUNCTION: LEMBALL 0x0045D2B0
static void LEMBALL_FASTCALL CopyBuffersIntoTwoArrayListResourceAndParse(
    void *pObject,
    void *pUnusedEdx,
    unsigned int *pSource,
    int pRelatedObject,
    unsigned int cbBuffer)
{
    unsigned int pTarget;
    unsigned int i;
    char *pFrom;
    char *pTo;

    (void)pUnusedEdx;
    if ((int)((char *)pObject - (char *)pRelatedObject) == -0x4c) {
        if (*(unsigned int *)((char *)pObject + 0x4c) == 0) {
            pTarget = ((MogLoadResourceArchive *)g_pMainResourceArchive)
                ->AllocateResourceDataBufferWithEviction(cbBuffer);
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
            pTarget = ((MogLoadResourceArchive *)g_pMainResourceArchive)
                ->AllocateResourceDataBufferWithEviction(cbBuffer);
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
static int LEMBALL_FASTCALL EnsureIntZrleListEntriesLoaded(void *pObject) {
    return ListNeedsAgeIncrement(pObject);
}

// FUNCTION: LEMBALL 0x0045D430
static void LEMBALL_FASTCALL LoadListEntryArraysFromArchive(void *pObject) {
    *(int *)((char *)pObject + 0x24) = 0;
}

// FUNCTION: LEMBALL 0x0045D540
static void LEMBALL_FASTCALL ReleaseListEntriesAndStreamBuffer(
    void *pObject, void *pUnusedEdx, int fReleaseMode)
{
    unsigned int pBuffer;

    (void)pUnusedEdx;
    pBuffer = *(unsigned int *)((char *)pObject + 0x38);
    if (pBuffer != 0) {
        ((MogLoadResourceArchive *)g_pResourceArchive)
            ->FreeResourceObjectDataBuffer(pBuffer, 1);
        *(unsigned int *)((char *)pObject + 0x38) = 0;
    }
    *(int *)((char *)pObject + 0x10) = 0;
    (void)fReleaseMode;
}

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

// FUNCTION: LEMBALL 0x0045D5C0
static void LEMBALL_FASTCALL LoadListEntriesFromStreamDescriptors(void *pObject, void *pUnused, int nArgument) {
    GameListResourceStreamInterface *pResource;
    unsigned int i;

    (void)pUnused;
    pResource = (GameListResourceStreamInterface *)pObject;
    if (pResource->PrepareListEntryStream() == 0) {
        return;
    }

    i = 0;
    if (*(unsigned int *)((char *)pObject + 0x6c) /
            *(unsigned int *)(*(int *)((char *)pObject + 0x48) + 4) == 0) {
        return;
    }

    do {
        pResource->LoadListEntryFromStream(i, nArgument);
        ++i;
    } while (i < *(unsigned int *)((char *)pObject + 0x6c) /
                         *(unsigned int *)(*(int *)((char *)pObject + 0x48) + 4));
}

// FUNCTION: LEMBALL 0x0045D7B0
void *ConstructIntZrleListResourceFromId(void *pObject, int nResourceId) {
    int *pListResource;

    pListResource = (int *)pObject;
    *(void **)pListResource = g_pCachedResourceObjectBaseDeleteVtable;
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

// FUNCTION: LEMBALL 0x0045DF20
void *ConstructTwoArrayListResourceFromId(void *pObject, int nResourceId) {
    int *pListResource;

    pListResource = (int *)pObject;
    *(void **)pListResource = g_pCachedResourceObjectBaseDeleteVtable;
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

// FUNCTION: LEMBALL 0x0045D610
void *LoadZrleOnlyListResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)((MogLoadResourceArchive *)g_pMainResourceArchive)
                          ->FindCachedResourceObjectById(nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_ListResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x7c);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = g_pCachedResourceObjectBaseDeleteVtable;
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

// FUNCTION: LEMBALL 0x0045D850
void *LoadListResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)((MogLoadResourceArchive *)g_pMainResourceArchive)
                          ->FindCachedResourceObjectById(nResourceId);
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

// FUNCTION: LEMBALL 0x0045DE70
void *LoadTwoArrayListResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)((MogLoadResourceArchive *)g_pMainResourceArchive)
                          ->FindCachedResourceObjectById(nResourceId);
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

// FUNCTION: LEMBALL 0x0045E210
void *LoadBitmapResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)((MogLoadResourceArchive *)g_pMainResourceArchive)
                          ->FindCachedResourceObjectById(nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_BitmapResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x54);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_pDestroyGRTSResourceAndFreeThunk;
        *(unsigned short *)((char *)pResourceObject + 0x4a) = 0;
        *(unsigned short *)(pResourceObject + 0x12) = 0;
        *(void **)pResourceObject = g_pBitmapResourceVtable;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        return FinalizeLoadedResourceObjectResult(pResourceObject);
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x0045E2B0
void *LoadZrleResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)((MogLoadResourceArchive *)g_pMainResourceArchive)
                          ->FindCachedResourceObjectById(nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_ZrleResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x54);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_pDestroyGRTSResourceAndFreeThunk;
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

// FUNCTION: LEMBALL 0x0045DD90
void *LoadPalResource(int nResourceId) {
    int *pResourceObject;

    pResourceObject = (int *)((MogLoadResourceArchive *)g_pMainResourceArchive)
                          ->FindCachedResourceObjectById(nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_GAME_PaletteResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (int *)AllocateVSMemBlock(0x4c);
    if (pResourceObject != 0) {
        *(void **)pResourceObject = g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject[6] = 0;
        *(void **)pResourceObject = g_pPaletteResourceVtable;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        return FinalizeLoadedResourceObjectResult(pResourceObject);
    }
    return FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x00447DB0
void LoadMainGameVariantZrleListResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GameMainGameVariantResourceBundleLoader *)pBundle;
    ((GameVariantResourceBundleLoadInterface *)pBundle)
        ->PrepareNextResourceLoad();
    pResourceBundle->m_ppZrleListResources[
        pResourceBundle->m_cLoadedZrleListResources] =
        LoadZrleOnlyListResource(nResourceId);
    ++pResourceBundle->m_cLoadedZrleListResources;
}

// FUNCTION: LEMBALL 0x00447E30
void LoadMainGameVariantListResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GameMainGameVariantResourceBundleLoader *)pBundle;
    ((GameVariantResourceBundleLoadInterface *)pBundle)
        ->PrepareNextResourceLoad();
    pResourceBundle->m_ppListResources[
        pResourceBundle->m_cLoadedListResources] =
        LoadListResource(nResourceId);
    ++pResourceBundle->m_cLoadedListResources;
}

// FUNCTION: LEMBALL 0x00447EB0
void LoadMainGameVariantBitmapResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GameMainGameVariantResourceBundleLoader *)pBundle;
    ((GameVariantResourceBundleLoadInterface *)pBundle)
        ->PrepareNextResourceLoad();
    pResourceBundle->m_ppBitmapResources[
        pResourceBundle->m_cLoadedBitmapResources] =
        LoadBitmapResource(nResourceId);
    ++pResourceBundle->m_cLoadedBitmapResources;
}

// FUNCTION: LEMBALL 0x00447F30
void LoadMainGameVariantPaletteResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GameMainGameVariantResourceBundleLoader *)pBundle;
    ((GameVariantResourceBundleLoadInterface *)pBundle)
        ->PrepareNextResourceLoad();
    pResourceBundle->m_ppPaletteResources[pResourceBundle->m_cPaletteResources] = LoadPalResource(nResourceId);
    ++pResourceBundle->m_cPaletteResources;
}

// FUNCTION: LEMBALL 0x00447FB0
void GameMainGameVariantResourceBundleLoader::LoadMainGameVariantStringResource(int nResourceId) {
    ((GameVariantResourceBundleLoadInterface *)this)
        ->PrepareNextResourceLoad();
    m_ppStringResources[m_cStringResources] = LoadStringResource(nResourceId);
    ++m_cStringResources;
}

// FUNCTION: LEMBALL 0x00448030
void LoadMainGameVariantTwoArrayListResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundleLoader *pResourceBundle;

    pResourceBundle = (GameMainGameVariantResourceBundleLoader *)pBundle;
    ((GameVariantResourceBundleLoadInterface *)pBundle)
        ->PrepareNextResourceLoad();
    pResourceBundle->m_ppTwoArrayListResources[pResourceBundle->m_cTwoArrayListResources] = LoadTwoArrayListResource(nResourceId);
    ++pResourceBundle->m_cTwoArrayListResources;
}
#include "ENGINE/CORE/SAFEVT.H"
