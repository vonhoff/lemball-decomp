#include "MainOptions2Drawer.h"

#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Frontend/Controls/GunController.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResBitmap.h"

#include <new.h>

// GLOBAL: LEMBALL 0x0049f578
unsigned long g_dwMainOptions2AnimIds[10] = {RES_NEWFRONT_ICONS_HIRES_ZOOM_OFF,
											 RES_NEWFRONT_ICONS_HIRES_ZOOM_ON,
											 RES_NEWFRONT_ICONS_HIRES_EFFECTS_OFF,
											 RES_NEWFRONT_ICONS_HIRES_EFFECTS_ON,
											 RES_NEWFRONT_ICONS_HIRES_MUSIC_OFF,
											 RES_NEWFRONT_ICONS_HIRES_MUSIC_ON,
											 RES_NEWFRONT_ICONS_HIRES_RETURN,
											 RES_NEWFRONT_ICONS_HIRES_KEYBOARD,
											 RES_NEWFRONT_ICONS_HIRES_VIDEO_ON,
											 RES_NEWFRONT_ICONS_HIRES_VIDEO_OFF};

// GLOBAL: LEMBALL 0x0049f5a0
unsigned long g_dwMainOptions2CompactAnimIds[10] = {RES_NEWFRONT_ICONS_LORES_ZOOM_OFF,
													RES_NEWFRONT_ICONS_LORES_ZOOM_ON,
													RES_NEWFRONT_ICONS_LORES_EFFECTS_OFF,
													RES_NEWFRONT_ICONS_LORES_EFFECTS_ON,
													RES_NEWFRONT_ICONS_LORES_MUSIC_OFF,
													RES_NEWFRONT_ICONS_LORES_MUSIC_ON,
													RES_NEWFRONT_ICONS_LORES_RETURN,
													RES_NEWFRONT_ICONS_LORES_KEYBOARD,
													RES_NEWFRONT_ICONS_LORES_VIDEO_ON,
													RES_NEWFRONT_ICONS_LORES_VIDEO_OFF};

// GLOBAL: LEMBALL 0x0049f5c8
int g_anMainOptions2ButtonLayout[12] = {32, 116, 480, 116, 48, 232, 464, 232, 64, 348, 416, 348};

// GLOBAL: LEMBALL 0x0049f5f8
int g_anMainOptions2CompactButtonLayout[12] = {16, 58, 240, 58, 24, 116, 232, 116, 32, 174, 208, 174};

// 68K 0x1080a69e __ct__19CMainOptions2DrawerFP14CMain2DDisplayP4CGDIRC7CVSRect
// FUNCTION: LEMBALL 0x00448ab0
MainOptions2Drawer::MainOptions2Drawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, 3, 0, 0, 0, 0, 0)
{
	if (g_nMusicAvailable == 0) {
		g_nMusicVolume = 0;
	}
	if (g_nEffectsAvailable == 0) {
		g_nEffectsVolume = 0;
	}
	if (g_nZoomAvailable == 0) {
		g_nZoomEnabled = 0;
	}
	if (g_nAnimationsAvailable == 0) {
		g_nAnimationsDisabled = 1;
	}
	m_disableZoom = (unsigned int) (g_nZoomEnabled == 0);
	m_disableAnimations = (unsigned int) g_nAnimationsDisabled;
	m_transitionState = 0;
	m_transitionPending = 0;
	g_nPendingEffectsVolume = g_nEffectsVolume;
	g_nPendingMusicVolume = g_nMusicVolume;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	Setup();
}

// 68K 0x1080a7cc Load__19CMainOptions2DrawerFv
// FUNCTION: LEMBALL 0x00448ba0
void MainOptions2Drawer::Load()
{
	unsigned long* zoomAnim;
	unsigned long* effectsAnim;
	unsigned long* musicAnim;
	unsigned long* navigationAnim;
	unsigned long* animationsAnim;
	unsigned int compactMode;
	void* storage;

	if (m_mode == 0) {
		m_buttonLayout = g_anMainOptions2ButtonLayout;
		zoomAnim = &g_dwMainOptions2AnimIds[0];
		effectsAnim = &g_dwMainOptions2AnimIds[2];
		musicAnim = &g_dwMainOptions2AnimIds[4];
		navigationAnim = &g_dwMainOptions2AnimIds[6];
		animationsAnim = &g_dwMainOptions2AnimIds[8];
	}
	else {
		m_buttonLayout = g_anMainOptions2CompactButtonLayout;
		zoomAnim = &g_dwMainOptions2CompactAnimIds[0];
		effectsAnim = &g_dwMainOptions2CompactAnimIds[2];
		musicAnim = &g_dwMainOptions2CompactAnimIds[4];
		navigationAnim = &g_dwMainOptions2CompactAnimIds[6];
		animationsAnim = &g_dwMainOptions2CompactAnimIds[8];
	}
	m_primitiveBundle.m_primitive.m_x = (short) (((int) m_display->m_rect.m_width - (int) m_backgroundBitmap->m_x) / 2);
	m_primitiveBundle.m_primitive.m_y = 0;
	m_primitiveBundle.m_primitive.m_resource = m_backgroundBitmap;
	m_primitiveBundle.m_primitive.m_flags = 0x800;
	m_primitiveBundle.m_primitive.m_remap = 0;
	storage = operator new(0x27c);
	if (storage == 0) {
		m_gunController = 0;
	}
	else {
		m_gunController = new (storage) GunController((GWnd*) m_display, m_gdi, 8, (unsigned char) m_mode);
	}
	compactMode = (unsigned int) (m_mode != 0);
	if (compactMode != 0) {
		m_disableZoom = 1;
	}
	if (m_gunController == 0) {
		return;
	}
	m_gunController->AddButton(m_buttonLayout[0],
							   m_buttonLayout[1],
							   zoomAnim,
							   0,
							   (int) compactMode,
							   1,
							   0,
							   &m_disableZoom,
							   0xacef0004);
	m_gunController
		->AddButton(m_buttonLayout[2], m_buttonLayout[3], animationsAnim, 0, 0, 1, 0, &m_disableAnimations, 0xacef0007);
	m_gunController->AddButton(m_buttonLayout[4],
							   m_buttonLayout[5],
							   effectsAnim,
							   0,
							   0,
							   1,
							   0,
							   &g_nPendingEffectsVolume,
							   0xacef0005);
	m_gunController
		->AddButton(m_buttonLayout[6], m_buttonLayout[7], musicAnim, 0, 0, 1, 0, &g_nPendingMusicVolume, 0xacef0006);
	m_gunController->AddButton(m_buttonLayout[10],
							   m_buttonLayout[11],
							   navigationAnim,
							   1,
							   0,
							   0,
							   0,
							   &m_transitionPending,
							   0xacef0008);
	m_gunController->SetGun(0);
	m_gunController->SetSpriteWindow();
}

// 68K 0x1080aaf8 UnLoad__19CMainOptions2DrawerFv
// FUNCTION: LEMBALL 0x00448dc0
void MainOptions2Drawer::UnLoad()
{
	if (m_gunController != 0) {
		delete m_gunController;
	}
}

// 68K 0x1080ab4a __dt__19CMainOptions2DrawerFv
// FUNCTION: LEMBALL 0x00448de0
MainOptions2Drawer::~MainOptions2Drawer()
{
	if (m_mode == 0) {
		g_nZoomEnabled = (int) (m_disableZoom == 0);
	}
	g_nAnimationsDisabled = m_disableAnimations;
	g_pSoundView->SetEffectsOn(g_nPendingEffectsVolume);
	g_pSoundView->SetMusicOn(g_nPendingMusicVolume);
	g_nMusicVolume = g_nPendingMusicVolume;
	g_nEffectsVolume = g_nPendingEffectsVolume;
	if (g_nMusicAvailable == 0) {
		g_nMusicVolume = 0;
	}
	if (g_nEffectsAvailable == 0) {
		g_nEffectsVolume = 0;
	}
	if (g_nZoomAvailable == 0) {
		g_nZoomEnabled = 0;
	}
	if (g_nAnimationsAvailable == 0) {
		g_nAnimationsDisabled = 1;
	}
	g_pSoundView->SetEffectsOn(g_nPendingEffectsVolume);
	g_pSoundView->SetMusicOn(g_nPendingMusicVolume);
	if (m_loaded != 0) {
		UnLoad();
	}
}

// 68K 0x1080ac8c ProcessMessages__19CMainOptions2DrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00448ee0
bool MainOptions2Drawer::ProcessMessages(Message* p_message)
{
	switch (p_message->type) {
	case 0xc:
		break;
	default:
		m_processedCount++;
		return 0;
	}

	switch ((unsigned int) p_message->code) {
	case 0xacef0005:
	case 0xacef0006:
		return 1;
	case 0xacef0008:
		m_quitYet = 1;
		m_returnState = 2;
		return 1;
	case 0xacef0009:
		m_quitYet = 1;
		m_returnState = 0x11;
		return 1;
	case 0xacff0000:
		g_nPendingEffectsVolume = (int) p_message->payload;
		g_pSoundView->SetEffectsVolume((unsigned char) (((unsigned int) p_message->payload * RES_MAPEDIT_FONT4X8) /
														(unsigned int) p_message->source));
		return 1;
	case 0xacff0001: {
		g_nPendingMusicVolume = (int) p_message->payload;
		unsigned char volume = (unsigned char) (((unsigned int) p_message->payload * RES_MAPEDIT_FONT4X8) /
												(unsigned int) p_message->source);
		*g_pSysOutput << "Setting music volume " << (char) volume << "\n";
		g_pSoundView->SetMusicVolume(volume);
		return 1;
	}
	default:
		return 0;
	}
}

// 68K 0x1080ada6 DrawBackGround__19CMainOptions2DrawerFv
// FUNCTION: LEMBALL 0x00449000
void MainOptions2Drawer::DrawBackGround()
{
}
