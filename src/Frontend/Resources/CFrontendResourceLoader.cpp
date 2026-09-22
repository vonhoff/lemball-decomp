#include "CFrontendResourceLoader.h"

#include "../../Control/Game/GameMain.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Resources/CMogRes.h"
#include "../../Visos/Resources/CResANIM.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/CResFONT.h"
#include "../../Visos/Resources/CResMOVIE.h"
#include "../../Visos/Resources/CResPALETTE.h"
#include "../../Visos/Resources/CResSTRING.h"
#include "../../Visos/Resources/Manifest.h"
#include "CCDLoadAnim.h"
class CCdLoadAnimDraw;
class CLoadUpdate;

// FUNCTION: LEMBALL 0x004479e0
CFrontendResourceLoader::CFrontendResourceLoader(CMain2DDisplay* p_display, int p_soundState)
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
	m_anims = (CResANIM**) operator new(m_animCapacity << 2);
	m_fonts = (CResFONT**) operator new(m_fontCapacity << 2);
	m_bitmaps = (CResBITMAP**) operator new(m_bitmapCapacity << 2);
	m_palettes = (CResPALETTE**) operator new(8);
	m_strings = (CResSTRING**) operator new(4);
	m_movies = (CResMOVIE**) operator new(0x18);
	m_loadAnim = new CCDLoadAnim(p_display->m_gdi, p_display);
	p_display->m_loadingDraw = static_cast<CCdLoadAnimDraw*>(m_loadAnim);
	m_totalResources += g_pSoundView->GetnEffects((unsigned short) p_soundState);
	m_loadAnim->InitialiseScreen();
	m_loadedResources = 0;
	g_pSoundView->ChangeState((unsigned short) p_soundState, (CLoadUpdate*) this);
	for (i = 0; i < (unsigned int) m_animCapacity; i++) {
		LoadANIM(m_animResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_fontCapacity; i++) {
		LoadFONT(m_fontResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_bitmapCapacity; i++) {
		LoadBITMAP(m_bitmapResourceIds[i]);
	}
	id = g_dwFrontendPaletteIds;
	do {
		LoadPALETTE(*id);
		++id;
	} while (id < g_dwFrontendPaletteIds + 2);
	id = g_dwFrontendStringIds;
	do {
		LoadSTRING(*id);
		++id;
	} while (id < g_dwFrontendStringIds + 1);
	for (i = 0; i < 3; i++) {
		LoadMOVIE(i + RES_NEWFRONT_STRINGS_AVINAMES_LORES_SUCCESS_SUCCESS1);
	}
	for (i = 0; i < 3; i++) {
		LoadMOVIE(i + RES_NEWFRONT_STRINGS_AVINAMES_LORES_FAIL_FAIL1);
	}
	p_display->m_loadingDraw = 0;
	if (m_loadAnim != 0) {
		delete m_loadAnim;
	}
}

// FUNCTION: LEMBALL 0x00447c50
CFrontendResourceLoader::~CFrontendResourceLoader()
{
	unsigned int i;
	unsigned int* id;

	g_pSoundView->ChangeState(0, 0);
	for (i = 0; i < (unsigned int) m_animCapacity; i++) {
		UnLoadANIM(m_animResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_fontCapacity; i++) {
		UnLoadFONT(m_fontResourceIds[i]);
	}
	for (i = 0; i < (unsigned int) m_bitmapCapacity; i++) {
		UnLoadBITMAP(m_bitmapResourceIds[i]);
	}
	id = g_dwFrontendPaletteIds;
	do {
		UnLoadPALETTE(*id);
		++id;
	} while (id < g_dwFrontendPaletteIds + 2);
	id = g_dwFrontendStringIds;
	do {
		UnLoadSTRING(*id);
		++id;
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

// FUNCTION: LEMBALL 0x00447d80
void CFrontendResourceLoader::UpdateNonCacheLoad()
{
	int loaded;

	loaded = m_loadedResources + 1;
	m_loadedResources = loaded;
	m_loadAnim->Draw((short) ((loaded * 100) / m_totalResources));
}

// FUNCTION: LEMBALL 0x00447db0
void CFrontendResourceLoader::LoadANIM(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_anims[m_loadedAnims] = CResANIM::Load(p_resourceId);
	m_loadedAnims = m_loadedAnims + 1;
}

// FUNCTION: LEMBALL 0x00447de0
void CFrontendResourceLoader::UnLoadANIM(unsigned long p_resourceId)
{
	CResANIM** slot;
	unsigned int i;

	for (i = 0; i < (unsigned int) m_loadedAnims; i++) {
		slot = &m_anims[i];
		if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
			m_anims[i]->UnLoad();
			m_anims[i] = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00447e30
void CFrontendResourceLoader::LoadFONT(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_fonts[m_loadedFonts] = CResFONT::Load(p_resourceId);
	m_loadedFonts = m_loadedFonts + 1;
}

// FUNCTION: LEMBALL 0x00447e60
void CFrontendResourceLoader::UnLoadFONT(unsigned long p_resourceId)
{
	unsigned int i;

	for (i = 0; i < (unsigned int) m_loadedFonts; i++) {
		if (m_fonts[i] != 0 && m_fonts[i]->m_resourceId == p_resourceId) {
			m_fonts[i]->UnLoad();
			m_fonts[i] = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00447eb0
void CFrontendResourceLoader::LoadBITMAP(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_bitmaps[m_loadedBitmaps] = CResBITMAP::Load(p_resourceId);
	m_loadedBitmaps = m_loadedBitmaps + 1;
}

// FUNCTION: LEMBALL 0x00447ee0
void CFrontendResourceLoader::UnLoadBITMAP(unsigned long p_resourceId)
{
	unsigned int i;

	for (i = 0; i < (unsigned int) m_loadedBitmaps; i++) {
		if (m_bitmaps[i] != 0 && m_bitmaps[i]->m_resourceId == p_resourceId) {
			m_bitmaps[i]->UnLoad();
			m_bitmaps[i] = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00447f30
void CFrontendResourceLoader::LoadPALETTE(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_palettes[m_loadedPalettes] = CResPALETTE::Load(p_resourceId);
	m_loadedPalettes = m_loadedPalettes + 1;
}

// FUNCTION: LEMBALL 0x00447f60
void CFrontendResourceLoader::UnLoadPALETTE(unsigned long p_resourceId)
{
	unsigned int i;

	for (i = 0; i < m_loadedPalettes; i++) {
		if (m_palettes[i] != 0 && m_palettes[i]->m_resourceId == p_resourceId) {
			m_palettes[i]->UnLoad();
			m_palettes[i] = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00447fb0
void CFrontendResourceLoader::LoadSTRING(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_strings[m_loadedStrings] = CResSTRING::Load(p_resourceId);
	m_loadedStrings = m_loadedStrings + 1;
}

// FUNCTION: LEMBALL 0x00447fe0
void CFrontendResourceLoader::UnLoadSTRING(unsigned long p_resourceId)
{
	CResSTRING** slot;
	unsigned int i;

	for (i = 0; i < (unsigned int) m_loadedStrings; i++) {
		slot = &m_strings[i];
		if (*slot != 0 && (*slot)->m_resourceId == p_resourceId) {
			m_strings[i]->UnLoad();
			m_strings[i] = 0;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00448030
void CFrontendResourceLoader::LoadMOVIE(unsigned long p_resourceId)
{
	UpdateNonCacheLoad();
	m_movies[m_loadedMovies] = CResMOVIE::Load(p_resourceId);
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
