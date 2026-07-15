#ifndef LEMBALL_VARIANT_RESOURCE_LOADER_H
#define LEMBALL_VARIANT_RESOURCE_LOADER_H

struct GameMainGameVariantResourceBundleLoader {
    void **m_pVtable;
    void *m_pStatusIndicatorManager;
    int m_nLoadedResourceCount;
    int m_nTotalResourceCount;
    void **m_ppZrleListResources;
    int m_cLoadedZrleListResources;
    int m_cZrleListResourceCapacity;
    void **m_ppListResources;
    int m_cLoadedListResources;
    int m_cListResourceCapacity;
    void **m_ppBitmapResources;
    int m_cLoadedBitmapResources;
    int m_cBitmapResourceCapacity;
    void **m_ppPaletteResources;
    int m_cPaletteResources;
    void **m_ppStringResources;
    int m_cStringResources;
    void **m_ppTwoArrayListResources;
    int m_cTwoArrayListResources;

    void LoadMainGameVariantStringResource(int nResourceId);
};

#endif
