#ifndef LEMBALL_VARIANT_RESOURCE_LOADER_H
#define LEMBALL_VARIANT_RESOURCE_LOADER_H

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

    void LoadMainGameVariantStringResource(int nResourceId);
};

#endif
