#ifndef LEMBALL_FRONTEND_RESOURCES_FRONTENDRESOURCELOADER_H
#define LEMBALL_FRONTEND_RESOURCES_FRONTENDRESOURCELOADER_H

#include "../../Common.h"

// SIZE 0x58
// VTABLE: LEMBALL 0x00497ad4
class FrontendResourceLoader {
public:
	FrontendResourceLoader(Main2DDisplay* p_arg0, int p_arg1);
	virtual void UpdateNonCacheLoad(); // vtable+0x00
	void LoadAnim(unsigned long p_resourceId);
	void LoadBitmap(unsigned long p_resourceId);
	void LoadFont(unsigned long p_resourceId);
	void LoadMovie(unsigned long p_resourceId);
	void LoadPalette(unsigned long p_resourceId);
	void LoadString(unsigned long p_resourceId);
	void UnLoadAnim(unsigned long p_resourceId);
	void UnLoadBitmap(unsigned long p_resourceId);
	void UnLoadFont(unsigned long p_resourceId);
	void UnLoadPalette(unsigned long p_resourceId);
	void UnLoadString(unsigned long p_resourceId);
	~FrontendResourceLoader();

private:
	CdLoadAnim* m_loadAnim;            // 0x04
	int m_loadedResources;             // 0x08
	int m_totalResources;              // 0x0c
	ResAnim** m_anims;                 // 0x10
	int m_loadedAnims;                 // 0x14
	int m_animCapacity;                // 0x18
	ResFont** m_fonts;                 // 0x1c
	int m_loadedFonts;                 // 0x20
	int m_fontCapacity;                // 0x24
	ResBitmap** m_bitmaps;             // 0x28
	int m_loadedBitmaps;               // 0x2c
	int m_bitmapCapacity;              // 0x30
	ResPalette** m_palettes;           // 0x34
	int m_loadedPalettes;              // 0x38
	ResString** m_strings;             // 0x3c
	int m_loadedStrings;               // 0x40
	ResMovie** m_movies;               // 0x44
	int m_loadedMovies;                // 0x48
	unsigned int* m_animResourceIds;   // 0x4c
	unsigned int* m_fontResourceIds;   // 0x50
	unsigned int* m_bitmapResourceIds; // 0x54
};

extern FrontendResourceLoader* g_pFrontendResourceLoader;
extern unsigned int g_dwGroundStyleResourceId;
extern unsigned int g_anGroundStyleResourceIds[10];
#endif
