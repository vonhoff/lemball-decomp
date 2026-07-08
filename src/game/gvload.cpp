#include "../game/game_app.h"
#include "../resource/resource_archive.h"
#include "../engine/memory_arena.h"

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

static void *g_GAME_ListResourceBaseVtable = (void *)0x004989c0;
static void *g_GAME_ZrleOnlyListResourceVtable = (void *)0x00498a18;
static void *g_GAME_IntZrleListResourceVtable = (void *)0x00498af0;
static void *g_GAME_TypedBitmapResourceDeleteVtable = (void *)0x00498ab0;
static void *g_GAME_ZrleResourceVtable = (void *)0x00498a70;
static void *g_GAME_BitmapResourceVtable = (void *)0x00498d20;
static void *g_GAME_PaletteResourceVtable = (void *)0x00498c08;
static void *g_GAME_TwoArrayListResourceVtable = (void *)0x00498c88;
static int DAT_004a1d64 = 0x004a1d64;
static int DAT_004a1d68 = 0x004a1d68;
static int DAT_004a1d6c = 0x004a1d6c;

extern void *g_pMainResourceArchive;
extern void *g_pCachedResourceObjectBaseDeleteVtable;
extern void InitializeResourceObjectFromId(void *pObject, int nResourceId);
extern void *FinalizeLoadedResourceObjectResult(void *pObject);

// FUNCTION: LEMBALL 0x0045C7B0
void *ConstructIntZrleListResourceFromId(void *pObject, int nResourceId) {
    int *pListResource;

    pListResource = (int *)pObject;
    *(void **)pListResource = &g_pCachedResourceObjectBaseDeleteVtable;
    pListResource[0x12] = DAT_004a1d68;
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

// FUNCTION: LEMBALL 0x0045CF20
void *ConstructTwoArrayListResourceFromId(void *pObject, int nResourceId) {
    int *pListResource;

    pListResource = (int *)pObject;
    *(void **)pListResource = &g_pCachedResourceObjectBaseDeleteVtable;
    pListResource[0x12] = DAT_004a1d6c;
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
        pResourceObject[0x12] = DAT_004a1d64;
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
