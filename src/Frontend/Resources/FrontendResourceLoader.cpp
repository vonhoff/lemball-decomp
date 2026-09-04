#include "FrontendResourceLoader.h"

#include "../../Control/Game/GameMain.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Resources/Manifest.h"
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
	unsigned int i;
	unsigned int* id;

	m_loadedMovies = 0;
	m_loadedStrings = 0;
	m_loadedPalettes = 0;
	m_loadedBitmaps = 0;
	m_loadedFonts = 0;
	m_loadedAnims = 0;
	if (g_nCompactPrimaryContextLayout != 0) {
		m_animResourceIds = g_dwFrontendCompactAnimIds;
		m_fontResourceIds = g_dwFrontendCompactFontIds;
		m_bitmapResourceIds = g_dwFrontendCompactBitmapIds;
		m_animCapacity = 0x44;
		m_fontCapacity = 1;
		m_bitmapCapacity = 3;
		m_totalResources = 0x48;
	}
	else {
		m_animResourceIds = g_dwFrontendAnimIds;
		m_fontResourceIds = g_dwFrontendFontIds;
		m_bitmapResourceIds = g_dwFrontendBitmapIds;
		m_animCapacity = 0x43;
		m_fontCapacity = 1;
		m_bitmapCapacity = 3;
		m_totalResources = 0x47;
	}
	m_totalResources += 3;
	m_totalResources += 3;
	m_totalResources += 3;
	m_anims = (ResAnim**) operator new(m_animCapacity << 2);
	m_fonts = (ResFont**) operator new(m_fontCapacity << 2);
	m_bitmaps = (ResBitmap**) operator new(m_bitmapCapacity << 2);
	m_palettes = (ResPalette**) operator new(8);
	m_strings = (ResString**) operator new(4);
	m_movies = (ResMovie**) operator new(0x18);
	m_loadAnim = new CdLoadAnim(p_arg0->m_gdi, p_arg0);
	if (m_loadAnim != 0) {
		p_arg0->m_drawPrimitive = &m_loadAnim->m_progress.m_draw;
	}
	else {
		p_arg0->m_drawPrimitive = 0;
	}
	m_totalResources += g_pSoundView->GetnEffects((unsigned short) p_arg1);
	m_loadAnim->InitialiseScreen();
	m_loadedResources = 0;
	g_pSoundView->ChangeState((unsigned short) p_arg1, (LoadUpdate*) this);
	for (i = 0; i < (unsigned int) m_animCapacity; i++) {
		LoadAnim(m_animResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_fontCapacity; i++) {
		LoadFont(m_fontResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_bitmapCapacity; i++) {
		LoadBitmap(m_bitmapResourceIds[i]);
	}
	id = g_dwFrontendPaletteIds;
	do {
		LoadPalette(*id++);
	} while (id < g_dwFrontendPaletteIds + 2);
	id = g_dwFrontendStringIds;
	do {
		LoadString(*id++);
	} while (id < g_dwFrontendStringIds + 1);
	for (i = 0; i < 3; i++) {
		LoadMovie(i + RES_NEWFRONT_STRINGS_AVINAMES_LORES_SUCCESS_SUCCESS1);
	}
	for (i = 0; i < 3; i++) {
		LoadMovie(i + RES_NEWFRONT_STRINGS_AVINAMES_LORES_FAIL_FAIL1);
	}
	p_arg0->m_drawPrimitive = 0;
	if (m_loadAnim != 0) {
		delete m_loadAnim;
	}
}

// 68K 0x1080d98e __dt__23CFrontendResourceLoaderFv
// FUNCTION: LEMBALL 0x00447c50
FrontendResourceLoader::~FrontendResourceLoader()
{
	unsigned int i;
	unsigned int* id;

	g_pSoundView->ChangeState(0, 0);
	for (i = 0; i < (unsigned int) m_animCapacity; i++) {
		UnLoadAnim(m_animResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_fontCapacity; i++) {
		UnLoadFont(m_fontResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_bitmapCapacity; i++) {
		UnLoadBitmap(m_bitmapResourceIds[i]);
	}
	id = g_dwFrontendPaletteIds;
	do {
		UnLoadPalette(*id++);
	} while (id < g_dwFrontendPaletteIds + 2);
	id = g_dwFrontendStringIds;
	do {
		UnLoadString(*id++);
	} while (id < g_dwFrontendStringIds + 1);
	for (i = 0; i < 6; i++) {
		m_movies[i]->UnLoad();
		m_movies[i] = 0;
	}
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
	unsigned int i;
	unsigned int count;
	ResAnim** anims;
	ResAnim** slot;

	i = 0;
	count = m_loadedAnims;
	if (count != 0) {
		anims = m_anims;
		slot = anims;
		do {
			if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
				anims[i]->UnLoad();
				anims[i] = 0;
				break;
			}
			slot++;
			i++;
		} while (i < count);
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

// 68K 0x1080ddfa UnLoadFONT__23CFrontendResourceLoaderFUl
// FUNCTION: LEMBALL 0x00447e60
void FrontendResourceLoader::UnLoadFont(unsigned long p_resourceId)
{
	unsigned int i;
	ResFont** slot;

	i = 0;
	if (m_loadedFonts != 0) {
		slot = m_fonts;
		do {
			if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
				m_fonts[i]->UnLoad();
				m_fonts[i] = 0;
				break;
			}
			slot++;
			i++;
		} while (i < (unsigned int) m_loadedFonts);
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
	unsigned int i;
	ResBitmap** slot;

	i = 0;
	if (m_loadedBitmaps != 0) {
		slot = m_bitmaps;
		do {
			if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
				m_bitmaps[i]->UnLoad();
				m_bitmaps[i] = 0;
				break;
			}
			slot++;
			i++;
		} while (i < (unsigned int) m_loadedBitmaps);
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
	unsigned int i = 0;
	if (m_loadedPalettes != 0) {
		ResPalette** slot = m_palettes;
		while (1) {
			if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
				m_palettes[i]->UnLoad();
				m_palettes[i] = 0;
				return;
			}
			slot++;
			i++;
			if (i >= (unsigned int) m_loadedPalettes) {
				return;
			}
		}
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
	unsigned int i;
	ResString** slot;

	i = 0;
	if (m_loadedStrings != 0) {
		slot = m_strings;
		do {
			if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
				m_strings[i]->UnLoad();
				m_strings[i] = 0;
				break;
			}
			slot++;
			i++;
		} while (i < (unsigned int) m_loadedStrings);
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
unsigned int g_dwFrontendCompactAnimIds[68] = {RES_NEWFRONT_ANIMS_LORES_BULLET_LEFT,
											   RES_NEWFRONT_ANIMS_LORES_BULLET_RIGHT,
											   RES_NEWFRONT_ANIMS_LORES_FIRE_LEFT,
											   RES_NEWFRONT_ANIMS_LORES_FIRE_RIGHT,
											   RES_NEWFRONT_ANIMS_LORES_FX_LEFT,
											   RES_NEWFRONT_ANIMS_LORES_FX_RIGHT,
											   RES_NEWFRONT_ANIMS_LORES_GUNTURN,
											   RES_NEWFRONT_ANIMS_LORES_SPLAT_LEFT,
											   RES_NEWFRONT_ANIMS_LORES_SPLAT_RIGHT,
											   RES_NEWFRONT_ICONS_LORES_ONE_PLAYER,
											   RES_NEWFRONT_ICONS_LORES_TWO_PLAYER,
											   RES_NEWFRONT_ICONS_LORES_OPTIONS,
											   RES_NEWFRONT_ICONS_LORES_FLOPPY_DISK,
											   RES_NEWFRONT_ICONS_LORES_SKILL_FUN,
											   RES_NEWFRONT_ICONS_LORES_SKILL_TRICKY,
											   RES_NEWFRONT_ICONS_LORES_SKILL_TAXING,
											   RES_NEWFRONT_ICONS_LORES_SKILL_MAYHEM,
											   RES_NEWFRONT_ICONS_LORES_OKAY,
											   RES_NEWFRONT_ICONS_LORES_FULLSCREEN,
											   RES_NEWFRONT_ICONS_LORES_PASSWORD,
											   RES_NEWFRONT_ICONS_LORES_MEMORY_LOAD,
											   RES_NEWFRONT_ICONS_LORES_MEMORY_SAVE,
											   RES_NEWFRONT_ICONS_LORES_FLOPPY_LOAD,
											   RES_NEWFRONT_ICONS_LORES_FLOPPY_SAVE,
											   RES_NEWFRONT_ANIMS_LORES_HILITE,
											   RES_NEWFRONT_ICONS_LORES_ZOOM_ON,
											   RES_NEWFRONT_ICONS_LORES_ZOOM_OFF,
											   RES_NEWFRONT_ICONS_LORES_EFFECTS,
											   RES_NEWFRONT_ICONS_LORES_EFFECTS_OFF,
											   RES_NEWFRONT_ICONS_LORES_EFFECTS_ON,
											   RES_NEWFRONT_ICONS_LORES_MUSIC_OFF,
											   RES_NEWFRONT_ICONS_LORES_MUSIC_ON,
											   RES_NEWFRONT_ICONS_LORES_MUSIC,
											   RES_NEWFRONT_ICONS_LORES_RETURN,
											   RES_NEWFRONT_ICONS_LORES_CANCEL,
											   RES_NEWFRONT_ICONS_LORES_PSX_CONTROLLER,
											   RES_NEWFRONT_ICONS_LORES_KEYBOARD,
											   RES_NEWFRONT_ICONS_LORES_VIDEO_ON,
											   RES_NEWFRONT_ICONS_LORES_VIDEO_OFF,
											   RES_NEWFRONT_ICONS_LORES_START_LOCAL,
											   RES_NEWFRONT_ICONS_LORES_START_SPEC,
											   RES_NEWFRONT_ICONS_LORES_LEFT_ARROW,
											   RES_NEWFRONT_ICONS_LORES_RIGHT_ARROW,
											   RES_NEWFRONT_ICONS_LORES_LEFT_ARROW_DISABLED,
											   RES_NEWFRONT_ICONS_LORES_RIGHT_ARROW_DISABLED,
											   RES_NEWFRONT_ANIMS_LORES_DANCE,
											   RES_NEWFRONT_ANIMS_LORES_FLAG,
											   RES_NEWFRONT_ANIMS_LORES_GUNLEMM_EYES,
											   RES_NEWFRONT_ANIMS_LORES_CLOCK,
											   RES_NEWFRONT_ANIMS_LORES_FRAME_1,
											   RES_NEWFRONT_ANIMS_LORES_FRAME_2,
											   RES_NEWFRONT_ANIMS_LORES_FRAME_3,
											   RES_NEWFRONT_ANIMS_LORES_SUCCESS_EYES,
											   RES_NEWFRONT_ANIMS_LORES_FAIL_EYES,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_0,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_1,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_2,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_3,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_4,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_5,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_6,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_7,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_8,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_9,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_END,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_CLR,
											   RES_NEWFRONT_ANIMS_LORES_PASSWORD_HILITE,
											   RES_NEWFRONT_ICONS_LORES_MEMORY_CARD};

// GLOBAL: LEMBALL 0x0049f2d0
unsigned int g_dwFrontendAnimIds[67] = {RES_NEWFRONT_ANIMS_HIRES_BULLET_LEFT,
										RES_NEWFRONT_ANIMS_HIRES_BULLET_RIGHT,
										RES_NEWFRONT_ANIMS_HIRES_FIRE_LEFT,
										RES_NEWFRONT_ANIMS_HIRES_FIRE_RIGHT,
										RES_NEWFRONT_ANIMS_HIRES_FX_LEFT,
										RES_NEWFRONT_ANIMS_HIRES_FX_RIGHT,
										RES_NEWFRONT_ANIMS_HIRES_GUNTURN,
										RES_NEWFRONT_ANIMS_HIRES_SPLAT_LEFT,
										RES_NEWFRONT_ANIMS_HIRES_SPLAT_RIGHT,
										RES_NEWFRONT_ICONS_HIRES_ONE_PLAYER,
										RES_NEWFRONT_ICONS_HIRES_TWO_PLAYER,
										RES_NEWFRONT_ICONS_HIRES_OPTIONS,
										RES_NEWFRONT_ICONS_HIRES_FLOPPY_DISK,
										RES_NEWFRONT_ICONS_HIRES_SKILL_FUN,
										RES_NEWFRONT_ICONS_HIRES_SKILL_TRICKY,
										RES_NEWFRONT_ICONS_HIRES_SKILL_TAXING,
										RES_NEWFRONT_ICONS_HIRES_SKILL_MAYHEM,
										RES_NEWFRONT_ICONS_HIRES_OKAY,
										RES_NEWFRONT_ICONS_HIRES_FULLSCREEN,
										RES_NEWFRONT_ICONS_HIRES_PASSWORD,
										RES_NEWFRONT_ICONS_HIRES_MEMORY_LOAD,
										RES_NEWFRONT_ICONS_HIRES_MEMORY_SAVE,
										RES_NEWFRONT_ICONS_HIRES_FLOPPY_LOAD,
										RES_NEWFRONT_ICONS_HIRES_FLOPPY_SAVE,
										RES_NEWFRONT_ANIMS_HIRES_HILITE,
										RES_NEWFRONT_ICONS_HIRES_ZOOM_ON,
										RES_NEWFRONT_ICONS_HIRES_ZOOM_OFF,
										RES_NEWFRONT_ICONS_HIRES_EFFECTS,
										RES_NEWFRONT_ICONS_HIRES_MUSIC,
										RES_NEWFRONT_ICONS_HIRES_EFFECTS_OFF,
										RES_NEWFRONT_ICONS_HIRES_EFFECTS_ON,
										RES_NEWFRONT_ICONS_HIRES_MUSIC_OFF,
										RES_NEWFRONT_ICONS_HIRES_MUSIC_ON,
										RES_NEWFRONT_ICONS_HIRES_PSX_CONTROLLER,
										RES_NEWFRONT_ICONS_HIRES_KEYBOARD,
										RES_NEWFRONT_ICONS_HIRES_RETURN,
										RES_NEWFRONT_ICONS_HIRES_CANCEL,
										RES_NEWFRONT_ICONS_HIRES_VIDEO_ON,
										RES_NEWFRONT_ICONS_HIRES_VIDEO_OFF,
										RES_NEWFRONT_ICONS_HIRES_START_LOCAL,
										RES_NEWFRONT_ICONS_HIRES_START_SPEC,
										RES_NEWFRONT_ICONS_HIRES_LEFT_ARROW,
										RES_NEWFRONT_ICONS_HIRES_RIGHT_ARROW,
										RES_NEWFRONT_ICONS_HIRES_LEFT_ARROW_DISABLED,
										RES_NEWFRONT_ICONS_HIRES_RIGHT_ARROW_DISABLED,
										RES_NEWFRONT_ANIMS_HIRES_DANCE,
										RES_NEWFRONT_ANIMS_HIRES_FLAG,
										RES_NEWFRONT_ANIMS_HIRES_GUNLEMM_EYES,
										RES_NEWFRONT_ANIMS_HIRES_CLOCK,
										RES_NEWFRONT_ANIMS_HIRES_FRAME_1,
										RES_NEWFRONT_ANIMS_HIRES_FRAME_2,
										RES_NEWFRONT_ANIMS_HIRES_FRAME_3,
										RES_NEWFRONT_ANIMS_HIRES_SUCCESS_EYES,
										RES_NEWFRONT_ANIMS_HIRES_FAIL_EYES,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_0,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_1,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_2,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_3,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_4,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_5,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_6,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_7,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_8,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_9,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_END,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_CLR,
										RES_NEWFRONT_ANIMS_HIRES_PASSWORD_HILITE};

// GLOBAL: LEMBALL 0x0049f3dc
unsigned int g_dwFrontendCompactFontIds[1] = {RES_NEWFRONT_FONTS_LORES_CHALK_FONT};

// GLOBAL: LEMBALL 0x0049f3e0
unsigned int g_dwFrontendFontIds[1] = {RES_NEWFRONT_FONTS_HIRES_CHALK_FONT};

// GLOBAL: LEMBALL 0x0049f3e8
unsigned int g_dwFrontendCompactBitmapIds[3] = {RES_NEWFRONT_BITMAPS_LORES_PAINTBALL_TILE,
												RES_NEWFRONT_BITMAPS_LORES_TITLE_BMP,
												RES_NEWFRONT_BITMAPS_LORES_GUNLEMM};

// GLOBAL: LEMBALL 0x0049f3f8
unsigned int g_dwFrontendBitmapIds[3] = {RES_NEWFRONT_BITMAPS_HIRES_PAINTBALL_TILE,
										 RES_NEWFRONT_BITMAPS_HIRES_TITLE_BMP,
										 RES_NEWFRONT_BITMAPS_HIRES_GUNLEMM};

// GLOBAL: LEMBALL 0x0049f408
unsigned int g_dwFrontendPaletteIds[2] = {RES_PALETTES_TITLEPALETTE, RES_REGISTRATION_VISOS_PALETTE};

// GLOBAL: LEMBALL 0x0049f410
unsigned int g_dwFrontendStringIds[1] = {RES_REGISTRATION_FINGERPRINT};

// GLOBAL: LEMBALL 0x004a784c
unsigned int g_dwGroundStyleResourceId;

// GLOBAL: LEMBALL 0x004a786c
unsigned int g_anGroundStyleResourceIds[10];
