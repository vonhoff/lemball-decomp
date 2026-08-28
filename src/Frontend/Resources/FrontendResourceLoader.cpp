#include "FrontendResourceLoader.h"

#include "../../Control/Game/GameMain.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Resources/ResAnim.h"
#include "../../Visos/Resources/ResBase.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../../Visos/Resources/ResFont.h"
#include "../../Visos/Resources/ResMovie.h"
#include "../../Visos/Resources/ResPalette.h"
#include "../../Visos/Resources/ResString.h"
#include "CdLoadAnim.h"

#include <new.h>

// 68K 0x1080d5a8 __ct__23CFrontendResourceLoaderFP14CMain2DDisplayi
// FUNCTION: LEMBALL 0x004479e0
FrontendResourceLoader::FrontendResourceLoader(Main2DDisplay* p_arg0, int p_arg1)
{
	void* storage;
	int total;
	unsigned int i;
	int offset;
	unsigned int* id;
	int effects;

	m_loadedMovies = 0;
	m_loadedStrings = 0;
	m_loadedPalettes = 0;
	m_loadedBitmaps = 0;
	m_loadedFonts = 0;
	m_loadedAnims = 0;
	if (g_nCompactPrimaryContextLayout == 0) {
		m_animResourceIds = g_dwFrontendAnimIds;
		m_fontResourceIds = g_dwFrontendFontIds;
		m_bitmapResourceIds = g_dwFrontendBitmapIds;
		m_animCapacity = 0x43;
		m_fontCapacity = 1;
		m_bitmapCapacity = 3;
		m_totalResources = 0x47;
	}
	else {
		m_animResourceIds = g_dwFrontendCompactAnimIds;
		m_fontResourceIds = g_dwFrontendCompactFontIds;
		m_bitmapResourceIds = g_dwFrontendCompactBitmapIds;
		m_animCapacity = 0x44;
		m_fontCapacity = 1;
		m_bitmapCapacity = 3;
		m_totalResources = 0x48;
	}
	total = m_totalResources;
	m_totalResources = total + 3;
	m_totalResources = total + 6;
	m_totalResources = total + 9;
	m_anims = (ResAnim**) operator new(m_animCapacity << 2);
	m_fonts = (ResFont**) operator new(m_fontCapacity << 2);
	m_bitmaps = (ResBitmap**) operator new(m_bitmapCapacity << 2);
	m_palettes = (ResPalette**) operator new(8);
	m_strings = (ResString**) operator new(4);
	m_movies = (ResMovie**) operator new(0x18);
	storage = operator new(0x134);
	if (storage == 0) {
		m_loadAnim = 0;
	}
	else {
		m_loadAnim = new (storage) CdLoadAnim(p_arg0->m_gdi, p_arg0);
	}
	if (m_loadAnim == 0) {
		p_arg0->m_drawPrimitive = 0;
	}
	else {
		p_arg0->m_drawPrimitive = &m_loadAnim->m_progress.m_draw;
	}
	effects = g_pSoundView->GetnEffects((unsigned short) p_arg1);
	m_totalResources = m_totalResources + effects;
	i = 0;
	m_loadAnim->InitialiseScreen();
	m_loadedResources = 0;
	g_pSoundView->ChangeState((unsigned short) p_arg1, (LoadUpdate*) this);
	if (m_animCapacity != 0) {
		offset = 0;
		do {
			i = i + 1;
			LoadAnim(*(unsigned int*) ((int) m_animResourceIds + offset));
			offset = offset + 4;
		} while (i < (unsigned int) m_animCapacity);
	}
	i = 0;
	offset = 0;
	if (m_fontCapacity != 0) {
		do {
			i = i + 1;
			LoadFont(*(unsigned int*) ((int) m_fontResourceIds + offset));
			offset = offset + 4;
		} while (i < (unsigned int) m_fontCapacity);
	}
	i = 0;
	offset = 0;
	if (m_bitmapCapacity != 0) {
		do {
			i = i + 1;
			LoadBitmap(*(unsigned int*) ((int) m_bitmapResourceIds + offset));
			offset = offset + 4;
		} while (i < (unsigned int) m_bitmapCapacity);
	}
	id = g_dwFrontendPaletteIds;
	do {
		i = *id;
		id = id + 1;
		LoadPalette(i);
	} while (id < g_dwFrontendPaletteIds + 2);
	id = g_dwFrontendStringIds;
	do {
		i = *id;
		id = id + 1;
		LoadString(i);
	} while (id < g_dwFrontendStringIds + 1);
	i = 0;
	do {
		LoadMovie(i + 0x11e);
		i = i + 1;
	} while (i < 3);
	i = 0;
	do {
		LoadMovie(i + 0x121);
		i = i + 1;
	} while (i < 3);
	p_arg0->m_drawPrimitive = 0;
	if (m_loadAnim != 0) {
		m_loadAnim->~CdLoadAnim();
		operator delete(m_loadAnim);
	}
}

// 68K 0x1080d98e __dt__23CFrontendResourceLoaderFv
// FUNCTION: LEMBALL 0x00447c50
FrontendResourceLoader::~FrontendResourceLoader()
{
	unsigned int i;
	int offset;
	unsigned int* id;
	unsigned int next;

	g_pSoundView->ChangeState(0, 0);
	i = 0;
	if (m_animCapacity != 0) {
		offset = 0;
		do {
			i = i + 1;
			UnLoadAnim(*(unsigned int*) ((int) m_animResourceIds + offset));
			offset = offset + 4;
		} while (i < (unsigned int) m_animCapacity);
	}
	i = 0;
	if (m_fontCapacity != 0) {
		offset = 0;
		do {
			i = i + 1;
			UnLoadFont(*(unsigned int*) ((int) m_fontResourceIds + offset));
			offset = offset + 4;
		} while (i < (unsigned int) m_fontCapacity);
	}
	i = 0;
	offset = 0;
	if (m_bitmapCapacity != 0) {
		do {
			i = i + 1;
			UnLoadBitmap(*(unsigned int*) ((int) m_bitmapResourceIds + offset));
			offset = offset + 4;
		} while (i < (unsigned int) m_bitmapCapacity);
	}
	id = g_dwFrontendPaletteIds;
	do {
		i = *id;
		id = id + 1;
		UnLoadPalette(i);
	} while (id < g_dwFrontendPaletteIds + 2);
	id = g_dwFrontendStringIds;
	do {
		i = *id;
		id = id + 1;
		UnLoadString(i);
	} while (id < g_dwFrontendStringIds + 1);
	i = 0;
	do {
		next = i + 4;
		(*(ResBase**) ((int) m_movies + i))->UnLoad();
		*(ResMovie**) ((int) m_movies + i) = 0;
		i = next;
	} while (next < 0x18);
	operator delete(m_movies);
	operator delete(m_strings);
	operator delete(m_palettes);
	operator delete(m_bitmaps);
	operator delete(m_fonts);
	operator delete(m_anims);
	g_pMogRes->CleanUpResources();
}

// 68K 0x1080db62 UpdateNonCacheLoad__23CFrontendResourceLoaderFv
// FUNCTION: LEMBALL 0x00447d80
void FrontendResourceLoader::UpdateNonCacheLoad()
{
	int loaded;

	loaded = m_loadedResources + 1;
	m_loadedResources = loaded;
	m_loadAnim->m_progress.Draw((short) ((loaded * 100) / m_totalResources));
}

// 68K 0x1080dbca LoadANIM__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447db0
void FrontendResourceLoader::LoadAnim(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_anims[m_loadedAnims] = ResAnim::Load(p_resourceId);
	m_loadedAnims = m_loadedAnims + 1;
}

// 68K 0x1080dc2c UnLoadANIM__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447de0
void FrontendResourceLoader::UnLoadAnim(unsigned long p_resourceId)
{
	ResAnim** slot;
	unsigned int i;

	i = 0;
	if (m_loadedAnims != 0) {
		slot = m_anims;
		while (*slot == 0 || (*slot)->m_resourceId != p_resourceId) {
			slot = slot + 1;
			i = i + 1;
			if ((unsigned int) m_loadedAnims <= i) {
				return;
			}
		}
		m_anims[i]->UnLoad();
		m_anims[i] = 0;
	}
}

// 68K 0x1080dd96 LoadFONT__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447e30
void FrontendResourceLoader::LoadFont(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_fonts[m_loadedFonts] = ResFont::Load(p_resourceId);
	m_loadedFonts = m_loadedFonts + 1;
}

// 68K 0x1080ddf8 UnLoadFONT__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447e60
void FrontendResourceLoader::UnLoadFont(unsigned long p_resourceId)
{
	ResFont** slot;
	unsigned int i;

	i = 0;
	if (m_loadedFonts != 0) {
		slot = m_fonts;
		while (*slot == 0 || (*slot)->m_resourceId != p_resourceId) {
			slot = slot + 1;
			i = i + 1;
			if ((unsigned int) m_loadedFonts <= i) {
				return;
			}
		}
		m_fonts[i]->UnLoad();
		m_fonts[i] = 0;
	}
}

// 68K 0x1080de7c LoadBITMAP__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447eb0
void FrontendResourceLoader::LoadBitmap(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_bitmaps[m_loadedBitmaps] = ResBitmap::Load(p_resourceId);
	m_loadedBitmaps = m_loadedBitmaps + 1;
}

// 68K 0x1080dee0 UnLoadBITMAP__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447ee0
void FrontendResourceLoader::UnLoadBitmap(unsigned long p_resourceId)
{
	ResBitmap** slot;
	unsigned int i;

	i = 0;
	if (m_loadedBitmaps != 0) {
		slot = m_bitmaps;
		while (*slot == 0 || (*slot)->m_resourceId != p_resourceId) {
			slot = slot + 1;
			i = i + 1;
			if ((unsigned int) m_loadedBitmaps <= i) {
				return;
			}
		}
		m_bitmaps[i]->UnLoad();
		m_bitmaps[i] = 0;
	}
}

// 68K 0x1080df66 LoadPALETTE__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447f30
void FrontendResourceLoader::LoadPalette(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_palettes[m_loadedPalettes] = ResPalette::Load(p_resourceId);
	m_loadedPalettes = m_loadedPalettes + 1;
}

// 68K 0x1080dfcc UnLoadPALETTE__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447f60
void FrontendResourceLoader::UnLoadPalette(unsigned long p_resourceId)
{
	ResPalette** slot;
	unsigned int i;

	i = 0;
	if (m_loadedPalettes != 0) {
		slot = m_palettes;
		while (*slot == 0 || (*slot)->m_resourceId != p_resourceId) {
			slot = slot + 1;
			i = i + 1;
			if ((unsigned int) m_loadedPalettes <= i) {
				return;
			}
		}
		m_palettes[i]->UnLoad();
		m_palettes[i] = 0;
	}
}

// 68K 0x1080e054 LoadSTRING__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447fb0
void FrontendResourceLoader::LoadString(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_strings[m_loadedStrings] = ResString::Load(p_resourceId);
	m_loadedStrings = m_loadedStrings + 1;
}

// 68K 0x1080e0b8 UnLoadSTRING__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447fe0
void FrontendResourceLoader::UnLoadString(unsigned long p_resourceId)
{
	ResString** slot;
	unsigned int i;

	i = 0;
	if (m_loadedStrings != 0) {
		slot = m_strings;
		while (*slot == 0 || (*slot)->m_resourceId != p_resourceId) {
			slot = slot + 1;
			i = i + 1;
			if ((unsigned int) m_loadedStrings <= i) {
				return;
			}
		}
		m_strings[i]->UnLoad();
		m_strings[i] = 0;
	}
}

// 68K 0x1080e13e LoadMOVIE__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00448030
void FrontendResourceLoader::LoadMovie(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_movies[m_loadedMovies] = ResMovie::Load(p_resourceId);
	m_loadedMovies = m_loadedMovies + 1;
}

// GLOBAL: LEMBALL 0x0049f1c0
unsigned int g_dwFrontendCompactAnimIds[68] = {
	0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17a, 0x17b, 0x1db, 0x1da, 0x1e4, 0x1dd, 0x1e0, 0x1e1, 0x1e2,
	0x1e3, 0x1ea, 0x1f8, 0x1f7, 0x1d8, 0x1d9, 0x1d6, 0x1d7, 0x183, 0x1e8, 0x1e9, 0x1ec, 0x208, 0x205, 0x206, 0x207,
	0x1ed, 0x1e7, 0x1eb, 0x1de, 0x1df, 0x1ef, 0x1f0, 0x1f3, 0x1f4, 0x1e5, 0x1e6, 0x1f1, 0x1f2, 0x180, 0x181, 0x17c,
	0x182, 0x17d, 0x17e, 0x17f, 0x185, 0x184, 0x186, 0x187, 0x188, 0x189, 0x18a, 0x18b, 0x18c, 0x18d, 0x18e, 0x18f,
	0x191, 0x190, 0x192, 0x1dc};

// GLOBAL: LEMBALL 0x0049f2d0
unsigned int g_dwFrontendAnimIds[67] = {
	0x146, 0x147, 0x148, 0x149, 0x14a, 0x14b, 0x14c, 0x14d, 0x14e, 0x1a7, 0x1a6, 0x1b0, 0x1a9, 0x1ac, 0x1ad, 0x1ae,
	0x1af, 0x1b6, 0x1c4, 0x1c3, 0x1a4, 0x1a5, 0x1a2, 0x1a3, 0x156, 0x1b4, 0x1b5, 0x1b8, 0x1b9, 0x1d5, 0x1d2, 0x1d3,
	0x1d4, 0x1aa, 0x1ab, 0x1b3, 0x1b7, 0x1bb, 0x1bc, 0x1bf, 0x1c0, 0x1b1, 0x1b2, 0x1bd, 0x1be, 0x153, 0x154, 0x14f,
	0x155, 0x150, 0x151, 0x152, 0x158, 0x157, 0x159, 0x15a, 0x15b, 0x15c, 0x15d, 0x15e, 0x15f, 0x160, 0x161, 0x162,
	0x164, 0x163, 0x165};

// GLOBAL: LEMBALL 0x0049f3dc
unsigned int g_dwFrontendCompactFontIds[1] = {0x114};

// GLOBAL: LEMBALL 0x0049f3e0
unsigned int g_dwFrontendFontIds[1] = {0x113};

// GLOBAL: LEMBALL 0x0049f3e8
unsigned int g_dwFrontendCompactBitmapIds[3] = {0x13c, 0x13d, 0x13e};

// GLOBAL: LEMBALL 0x0049f3f8
unsigned int g_dwFrontendBitmapIds[3] = {0x131, 0x132, 0x133};

// GLOBAL: LEMBALL 0x0049f408
unsigned int g_dwFrontendPaletteIds[2] = {0x10a, 0x102};

// GLOBAL: LEMBALL 0x0049f410
unsigned int g_dwFrontendStringIds[1] = {0x100};

// GLOBAL: LEMBALL 0x004a784c
unsigned int g_dwGroundStyleResourceId;

// GLOBAL: LEMBALL 0x004a786c
unsigned int g_anGroundStyleResourceIds[10];
