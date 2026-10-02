#include "CSuccFailDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Control/Game/GameMain.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Animation/CStatManager.h"
#include "../../Visos/Foundation/CArena.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/CVSOStream.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Base/CBaseFrontendProcess.h"
#include "../Controls/CHiliteController.h"
#include "Visos/Foundation/tagPRIMS.h"

extern "C" unsigned long __stdcall timeGetTime(void);

#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Messages/CNetworkGameMessage.h"
#include "../../Visos/Graphics/CBigBitmap.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Resources/CResFONT.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Support/CoordPair.h"
#include "Frontend/Windows/CSuccFailAnimWnd.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/CVSSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CPVGWnd.h"

#include <string.h>

class CGWnd;

#pragma intrinsic(strcpy, strlen)

// GLOBAL: LEMBALL 0x0049fcb4
char g_szSuccFailCollectedAllFlags[] = "You collected all the flags!";

// GLOBAL: LEMBALL 0x0049fcd4
char g_szSuccFailBeatScore[] = "You beat #'s score!";

// GLOBAL: LEMBALL 0x0049fce8
char g_szSuccFailCollectedAllYourFlags[] = "You collected all your flags!";

// GLOBAL: LEMBALL 0x0049fd08
char g_szSuccFailSplattedAllLemmings[] = "You splatted all #'s lemmings!";

// GLOBAL: LEMBALL 0x0049fd28
char g_szSuccFailRanOutOfTimeNet[] = "# ran out of time!";

// GLOBAL: LEMBALL 0x0049fd3c
char g_szSuccFailGaveUpNet[] = "# gave up!";

// GLOBAL: LEMBALL 0x0049fd48
char g_szSuccFailAllLemmingsEliminated[] = "All your lemmings have been eliminated!";

// GLOBAL: LEMBALL 0x0049fd74
char g_szSuccFailRanOutOfTimeSingle[] = "You ran out of time!";

// GLOBAL: LEMBALL 0x0049fd8c
char g_szSuccFailGaveUpSingle[] = "You gave up!";

// GLOBAL: LEMBALL 0x0049fd9c
char g_szSuccFailOpponentBeatScore[] = "# beat your score!";

// GLOBAL: LEMBALL 0x0049fdb4
char g_szSuccFailOpponentCollectedFlags[] = "# collected all the flags!";

// GLOBAL: LEMBALL 0x0049fdcc
char g_szSuccFailOpponentSplattedLemmings[] = "# splatted all your lemmings!";

// GLOBAL: LEMBALL 0x0049fdec
char g_szSuccFailRanOutOfTimeLose[] = "You ran out of time!";

// GLOBAL: LEMBALL 0x0049fe04
char g_szSuccFailGaveUpLose[] = "You gave up!";

// GLOBAL: LEMBALL 0x0049fe10
char g_szSuccFailMoviePrefix[] = "lemball";

// GLOBAL: LEMBALL 0x0049fb38
char* g_apSuccFailSingleWin[8] = {0, 0, g_szSuccFailCollectedAllFlags, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049fb58
char* g_apSuccFailNetWin[8] = {0,
							   g_szSuccFailBeatScore,
							   g_szSuccFailCollectedAllYourFlags,
							   g_szSuccFailSplattedAllLemmings,
							   g_szSuccFailRanOutOfTimeNet,
							   g_szSuccFailGaveUpNet,
							   0,
							   0};

// GLOBAL: LEMBALL 0x0049fb78
char* g_apSuccFailSingleLose[8] =
	{0, 0, 0, g_szSuccFailAllLemmingsEliminated, g_szSuccFailRanOutOfTimeSingle, g_szSuccFailGaveUpSingle, 0, 0};

// GLOBAL: LEMBALL 0x0049fb98
char* g_apSuccFailNetLose[8] = {0,
								g_szSuccFailOpponentBeatScore,
								g_szSuccFailOpponentCollectedFlags,
								g_szSuccFailOpponentSplattedLemmings,
								g_szSuccFailRanOutOfTimeLose,
								g_szSuccFailGaveUpLose,
								0,
								0};

// GLOBAL: LEMBALL 0x0049fbb8
SuccFailLayout g_succFailLayoutFull = {{92, 375},
									   {416, 375},
									   {0, 0},
									   {32, 146},
									   {145, 77},
									   {32, 146},
									   {147, 96},
									   {16, 80},
									   {272, 224},
									   {0, 6},
									   {32, 96},
									   {240, 192},
									   {0, 306}};

// GLOBAL: LEMBALL 0x0049fc20
SuccFailLayout g_succFailLayoutCompact = {{46, 190},
										  {208, 190},
										  {0, 0},
										  {16, 75},
										  {73, 41},
										  {16, 70},
										  {73, 44},
										  {8, 40},
										  {144, 112},
										  {0, 4},
										  {16, 48},
										  {128, 96},
										  {0, 153}};

// GLOBAL: LEMBALL 0x0049fc88
char g_szPasswordLabel[] = "Password: ";

// GLOBAL: LEMBALL 0x0049fc94
unsigned long g_dwSuccFailReturnAnimIdsFull = RES_NEWFRONT_ICONS_HIRES_RETURN;

// GLOBAL: LEMBALL 0x0049fc98
unsigned long g_dwSuccFailGoAnimIdsFull = RES_NEWFRONT_ICONS_HIRES_OKAY;

// GLOBAL: LEMBALL 0x0049fc9c
unsigned long g_dwSuccFailReturnAnimIdsCompact = RES_NEWFRONT_ICONS_LORES_RETURN;

// GLOBAL: LEMBALL 0x0049fca0
unsigned long g_dwSuccFailGoAnimIdsCompact = RES_NEWFRONT_ICONS_LORES_OKAY;

// GLOBAL: LEMBALL 0x0049fca4
unsigned long g_dwSuccFailFailureBitmapIdFull = RES_NEWFRONT_BITMAPS_HIRES_FAILURE_LEMMING;

// GLOBAL: LEMBALL 0x0049fca8
unsigned long g_dwSuccFailFailureBitmapIdCompact = RES_NEWFRONT_BITMAPS_LORES_FAILURE_LEMMING;

// GLOBAL: LEMBALL 0x0049fcac
unsigned long g_dwSuccFailSuccessBitmapIdFull = RES_NEWFRONT_BITMAPS_HIRES_SUCCESS_LEMMING;

// GLOBAL: LEMBALL 0x0049fcb0
unsigned long g_dwSuccFailSuccessBitmapIdCompact = RES_NEWFRONT_BITMAPS_LORES_SUCCESS_LEMMING;

// FUNCTION: LEMBALL 0x00450460
void CSuccFailDrawer::Load()
{
	unsigned long* returnAnim;
	unsigned long* goAnim;
	::tagPRIMS* primitive;
	tagPRIMS* primary;
	CResBITMAP* resource;
	unsigned int position;
	int bitmapX;
	int i;

	if (m_mode != 0) {
		m_layout = &g_succFailLayoutCompact;
		returnAnim = (unsigned long*) &g_dwSuccFailReturnAnimIdsCompact;
		goAnim = (unsigned long*) &g_dwSuccFailGoAnimIdsCompact;
		if (m_success != 0) {
			m_backgroundId = RES_NEWFRONT_ANIMS_LORES_SUCCESS_EYES;
			m_primaryBitmapId = g_dwSuccFailSuccessBitmapIdCompact;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_LORES_SUCCESS_BOARD;
		}
		else {
			m_primaryBitmapId = g_dwSuccFailFailureBitmapIdCompact;
			m_backgroundId = RES_NEWFRONT_ANIMS_LORES_FAIL_EYES;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_LORES_FAILURE_BOARD;
		}
	}
	else {
		m_layout = &g_succFailLayoutFull;
		returnAnim = (unsigned long*) &g_dwSuccFailReturnAnimIdsFull;
		goAnim = (unsigned long*) &g_dwSuccFailGoAnimIdsFull;
		unsigned int& primaryId = m_primaryBitmapId;
		if (m_success != 0) {
			unsigned int bitmapId = g_dwSuccFailSuccessBitmapIdFull;
			m_backgroundId = RES_NEWFRONT_ANIMS_HIRES_SUCCESS_EYES;
			primaryId = bitmapId;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_HIRES_SUCCESS_BOARD;
		}
		else {
			unsigned int bitmapId = g_dwSuccFailFailureBitmapIdFull;
			m_backgroundId = RES_NEWFRONT_ANIMS_HIRES_FAIL_EYES;
			primaryId = bitmapId;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_HIRES_FAILURE_BOARD;
		}
	}
	m_primaryBitmap = CResBITMAP::Load(m_primaryBitmapId);
	if (m_animationsEnabled == 0) {
		m_secondaryBitmap = CResBITMAP::Load(m_secondaryBitmapId);
	}
	else {
		m_secondaryBitmap = 0;
	}
	bitmapX = (int) m_width - (int) (short) m_primaryBitmap->m_x;
	primitive = m_primitiveBundle;
	primary = m_primitives;
	i = 1;
	do {
		resource = m_backgroundBitmap;
		position = m_layout->m_backgroundPosition.m_y;
		primitive->m_primitive.m_x = m_width - resource->m_x;
		primitive->m_primitive.m_y = position;
		primitive->m_primitive.m_resource = resource;
		primitive->m_primitive.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		primitive->m_primitive.m_remap = 0;
		resource = m_primaryBitmap;
		position = m_layout->m_primaryPosition.m_y;
		primary->m_primary.m_x = (short) bitmapX;
		primary->m_primary.m_y = position;
		primary->m_primary.m_resource = resource;
		primary->m_primary.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		primary->m_primary.m_remap = 0;
		CResBITMAP* secondaryResource = m_secondaryBitmap;
		if (secondaryResource != 0) {
			unsigned int secondaryY;
			SuccFailLayout* layout = m_layout;
			secondaryY = layout->m_secondaryPosition.m_y;
			unsigned int secondaryX = layout->m_secondaryPosition.m_x;
			primary->m_secondary.m_x = (short) secondaryX;
			primary->m_secondary.m_y = secondaryY;
			primary->m_secondary.m_resource = secondaryResource;
			primary->m_secondary.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
			primary->m_secondary.m_remap = 0;
		}
		primary++;
		primitive++;
	} while (--i != 0);
	m_layout->m_primaryPosition.m_x = bitmapX;
	m_layout->m_failurePosition.m_x = bitmapX;
	m_buttonBinding = 0;
	m_hiliteController = new CHiliteController((CGWnd*) m_display, m_gdi, 2, m_mode, 0);
	m_hiliteController->AddButton(m_layout->m_returnButton.m_x,
								  m_layout->m_returnButton.m_y,
								  goAnim,
								  1,
								  0,
								  0,
								  0,
								  &m_buttonBinding,
								  0xacef0010);
	m_hiliteController->AddButton(m_layout->m_goButton.m_x,
								  m_layout->m_goButton.m_y,
								  returnAnim,
								  1,
								  0,
								  0,
								  0,
								  &m_buttonBinding,
								  0xacef0011);
	m_hiliteController->SetHilite(0);
	m_hiliteController->SetHiliteWindow();
	short animY;
	short animX;
	if (m_success != 0) {
		animY = (short) m_layout->m_successAnimOffset.m_y + (short) m_layout->m_primaryPosition.m_y;
		animX = (short) m_layout->m_primaryPosition.m_x + (short) m_layout->m_successAnimOffset.m_x;
	}
	else {
		animY = (short) m_layout->m_failurePosition.m_y + (short) m_layout->m_failureAnimOffset.m_y;
		animX = (short) m_layout->m_failurePosition.m_x + (short) m_layout->m_failureAnimOffset.m_x;
	}
	m_animPosition.m_x = animX;
	m_animPosition.m_y = animY;
	CalculateText();
	if (m_animationsEnabled != 0) {
		m_animWindow.SetVariant(m_mode);
	}
}

// GLOBAL: LEMBALL 0x0049fe18
char g_szPaintballSequence[] = "Paintball Sequence";

// FUNCTION: LEMBALL 0x00450770
void CSuccFailDrawer::UnLoad()
{
	if (m_hiliteController != 0) {
		delete m_hiliteController;
	}
	m_primaryBitmap->UnLoad();
	if (m_secondaryBitmap != 0) {
		m_secondaryBitmap->UnLoad();
	}
}

// FUNCTION: LEMBALL 0x004507a0
CSuccFailDrawer::~CSuccFailDrawer()
{
	DestroyDrawer();
	if (m_loaded != 0) {
		UnLoad();
	}
	if (m_soundStopped == 0) {
		g_pSoundView->SetMusicOn(1);
		m_soundStopped = 1;
	}
}

// FUNCTION: LEMBALL 0x00450820
void CSuccFailDrawer::DestroyDrawer()
{
	if (m_animStarted != 0 && m_animWindow.m_lifecycleRefs == 1) {
		m_animWindow.Destroy();
		m_animStarted = 0;
		m_animStartDeadline = timeGetTime() + 0x28;
	}
}

// FUNCTION: LEMBALL 0x00450860
void CSuccFailDrawer::DrawText()
{
	CVSSize advance;

	if (m_drawingBackBuffer != 0) {
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi, m_firstLinePos, advance, m_chalkFontId, m_firstLine, 0x20, 0);
		if (m_secondLine != 0) {
			advance.m_height = 0;
			advance.m_width = 0;
			m_textManager->DrawString(m_gdi, m_secondLinePos, advance, m_chalkFontId, m_secondLine, 0x20, 0);
		}
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi, m_passwordLabelPos, advance, m_chalkFontId, g_szPasswordLabel, 0x20, 0);
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi, m_passwordPos, advance, m_chalkFontId, m_password, 0x20, 0);
	}
}

// FUNCTION: LEMBALL 0x00450970
bool CSuccFailDrawer::ProcessMessages(Message* p_message)
{
	switch ((unsigned int) p_message->m_type) {
	case 0xc:
		break;
	default:
		m_processedCount++;
		return 0;
	}

	switch ((unsigned int) p_message->m_code) {
	case 0xacef0010:
		if (m_networkMode != 0) {
			Action(USER_ACTION_3, USER_ACTION_STAGE_REQUEST);
			return 1;
		}
		Go();
		return 1;

	case 0xacef0011:
		if (m_networkMode != 0) {
			Action(USER_ACTION_2, USER_ACTION_STAGE_REQUEST);
			return 1;
		}
		Return();
		return 1;

	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x004509f0
void CSuccFailDrawer::Return()
{
	m_quitYet = 1;
	m_returnState = 2;
}

// FUNCTION: LEMBALL 0x00450a10
void CSuccFailDrawer::Go()
{
	if (g_nStatusDebugRequested != 0) {
		g_pStatManager->StreamOut(*g_pSysOutput);
	}
	if (g_nMemoryDebugRequested != 0) {
		g_pMasterArena->StreamOut(*g_pSysOutput);
	}
	m_quitYet = 1;
	m_returnState = 4;
}

// FUNCTION: LEMBALL 0x00450a60
bool CSuccFailDrawer::ConfirmedAction(eUserActions p_action)
{
	switch (p_action) {
	case USER_ACTION_2:
		Go();
		return 1;
	case USER_ACTION_3:
		Return();
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x00450a90
void CSuccFailDrawer::Processing()
{
	unsigned long now;
	SuccFailLayout* layout;

	if (m_animStarted == 0) {
		now = timeGetTime();
		if (now > m_animStartDeadline && m_animationsEnabled != 0) {
			if (m_display->IsWindowValid() != 0) {
				layout = m_layout;
				short rectHeight = (short) layout->m_animWindowEnd.m_y;
				short rectY = (short) layout->m_secondaryPosition.m_y;
				short rectX = (short) layout->m_secondaryPosition.m_x;
				CVSRect rect(rectX, rectY, (short) layout->m_animWindowEnd.m_x, rectHeight);
				m_animWindow.Create(rect, (CPVGWnd*) m_display, g_szPaintballSequence);
				m_animWindow.Play();
				m_animStarted = 1;
			}
		}
		if (m_animStarted == 0 && g_nAnimationsDisabled == 0) {
			goto sound;
		}
	}
	if (g_nTestAllLevels != 0) {
		int skill = g_pGameStatus->m_skill;
		if (skill != 3 || g_pGameStatus->m_lastLevels[skill] != 0x15) {
			Go();
		}
	}
sound:
	if (m_soundStarted == 0) {
		if (m_success != 0) {
			g_pSoundView->PlayEffect(SFX_SUCCESS);
		}
		else {
			g_pSoundView->PlayEffect(SFX_FAILURE);
		}
		m_soundStarted = 1;
		m_soundStartTime = timeGetTime();
	}
}

// FUNCTION: LEMBALL 0x00450ba0
void CSuccFailDrawer::DrawBackGround()
{
	SuccFailLayout* layout = m_layout;
	DrawFrame(layout->m_frameStart, layout->m_frameEnd);
	m_primitives[m_primitiveBank].m_primary.Draw(m_gdi);
	if (m_secondaryBitmap != 0) {
		m_primitives[m_primitiveBank].m_secondary.Draw(m_gdi);
	}
}

// FUNCTION: LEMBALL 0x00451110
CSuccFailDrawer::tagPRIMS::tagPRIMS()
{
}

// FUNCTION: LEMBALL 0x004511a0
CSuccFailDrawer::tagPRIMS::~tagPRIMS()
{
}
