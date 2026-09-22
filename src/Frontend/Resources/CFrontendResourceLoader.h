#ifndef LEMBALL_FRONTEND_RESOURCES_CFRONTENDRESOURCELOADER_H
#define LEMBALL_FRONTEND_RESOURCES_CFRONTENDRESOURCELOADER_H

#include "../../Views/Sound/CLoadUpdate.h" // complete type
#include "../../Views/Sound/CSoundView.h"  // complete type for CLoadUpdate

class CCDLoadAnim;
class CMain2DDisplay;
class CResANIM;
class CResBITMAP;
class CResFONT;
class CResMOVIE;
class CResPALETTE;
class CResSTRING;
// SIZE 0x58
// VTABLE: LEMBALL 0x00497ad4
class CFrontendResourceLoader : public CLoadUpdate {
public:
	CFrontendResourceLoader(CMain2DDisplay* p_display, int p_soundState);
	virtual void UpdateNonCacheLoad(); // vtable+0x00
	void LoadANIM(unsigned long p_resourceId);
	void LoadBITMAP(unsigned long p_resourceId);
	void LoadFONT(unsigned long p_resourceId);
	void LoadMOVIE(unsigned long p_resourceId);
	void LoadPALETTE(unsigned long p_resourceId);
	void LoadSTRING(unsigned long p_resourceId);
	void UnLoadANIM(unsigned long p_resourceId);
	void UnLoadBITMAP(unsigned long p_resourceId);
	void UnLoadFONT(unsigned long p_resourceId);
	void UnLoadPALETTE(unsigned long p_resourceId);
	void UnLoadSTRING(unsigned long p_resourceId);
	~CFrontendResourceLoader();

private:
	CCDLoadAnim* m_loadAnim;           // 0x04
	int m_loadedResources;             // 0x08
	int m_totalResources;              // 0x0c
	CResANIM** m_anims;                // 0x10
	int m_loadedAnims;                 // 0x14
	int m_animCapacity;                // 0x18
	CResFONT** m_fonts;                // 0x1c
	int m_loadedFonts;                 // 0x20
	int m_fontCapacity;                // 0x24
	CResBITMAP** m_bitmaps;            // 0x28
	int m_loadedBitmaps;               // 0x2c
	int m_bitmapCapacity;              // 0x30
	CResPALETTE** m_palettes;          // 0x34
	unsigned int m_loadedPalettes;     // 0x38
	CResSTRING** m_strings;            // 0x3c
	int m_loadedStrings;               // 0x40
	CResMOVIE** m_movies;              // 0x44
	int m_loadedMovies;                // 0x48
	unsigned int* m_animResourceIds;   // 0x4c
	unsigned int* m_fontResourceIds;   // 0x50
	unsigned int* m_bitmapResourceIds; // 0x54
};

extern unsigned int g_dwFrontendCompactAnimIds[68];
extern unsigned int g_dwFrontendAnimIds[67];
extern unsigned int g_dwFrontendCompactFontIds[1];
extern unsigned int g_dwFrontendFontIds[1];
extern unsigned int g_dwFrontendCompactBitmapIds[3];
extern unsigned int g_dwFrontendBitmapIds[3];
extern unsigned int g_dwFrontendPaletteIds[2];
extern unsigned int g_dwFrontendStringIds[1];
extern unsigned int g_dwGroundStyleResourceId;
extern unsigned int g_anGroundStyleResourceIds[10];
#endif
