#include "CSuccFailDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Control/Game/GameMain.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Animation/CStatManager.h"
#include "../../Visos/Foundation/CArena.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/CVsOStream.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Resources/CResBitmap.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Base/CBaseFrontendProcess.h"
#include "../Controls/CHiliteController.h"

extern "C" unsigned long __stdcall timeGetTime(void);

#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Messages/CNetworkGameMessage.h"
#include "../../Visos/Graphics/BitmapRes.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Resources/CResFont.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Drawers/CSuccFailDrawerPrims.h"
#include "Frontend/Support/CoordPair.h"
#include "Frontend/Windows/SuccFailAnimWindow.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/Prims.h"
#include "Visos/Graphics/CPvGWnd.h"

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
unsigned char g_abSuccFailLayoutFull[0x68] = {
	0x5c, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00,
	0x4d, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x93, 0x00, 0x00, 0x00, 0x60, 0x00,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x00,
	0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049fc20
unsigned char g_abSuccFailLayoutCompact[0x68] = {
	0x2e, 0x00, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00,
	0x29, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x2c, 0x00,
	0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x00,
	0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00,
};

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
unsigned long g_dwSuccFailSingleWinBitmapIdFull = RES_NEWFRONT_BITMAPS_HIRES_FAILURE_LEMMING;

// GLOBAL: LEMBALL 0x0049fca8
unsigned long g_dwSuccFailSingleWinBitmapIdCompact = RES_NEWFRONT_BITMAPS_LORES_FAILURE_LEMMING;

// GLOBAL: LEMBALL 0x0049fcac
unsigned long g_dwSuccFailSingleLoseBitmapIdFull = RES_NEWFRONT_BITMAPS_HIRES_SUCCESS_LEMMING;

// GLOBAL: LEMBALL 0x0049fcb0
unsigned long g_dwSuccFailSingleLoseBitmapIdCompact = RES_NEWFRONT_BITMAPS_LORES_SUCCESS_LEMMING;

// FUNCTION: LEMBALL 0x00450020
CSuccFailDrawer::CSuccFailDrawer(CMain2DDisplay* p_arg0, CGdi* p_arg1, const CVsRect& p_arg2, unsigned int p_arg3)
	: CBaseFrontendDrawer(p_arg0, p_arg1, p_arg2, FLOW_SUCCESS, 0x32, 200, 0, 0x28, 0x30)
{
	m_variant = p_arg3;
	m_animationsEnabled = (unsigned int) (g_nAnimationsDisabled == 0);
	m_animWindow.Initialise(this, m_display, p_arg3);
	m_animWindow.m_resolveMoviePath = 1;
	m_animWindow.m_moviePrefix = g_szSuccFailMoviePrefix;
	m_animWindow.m_useMoviePrefix = 1;
	m_animStarted = 0;
	m_soundStarted = 0;
	m_soundStopped = 0;
	m_animStartDeadline = CurrentMilliTimer() + 0x28;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	m_password = g_pGameStatus->EncodePassword();
	Setup();
}

// FUNCTION: LEMBALL 0x00450160
void CSuccFailDrawer::CalculateText()
{
	CResFont* font;
	char* format;
	char* hash;

	font = m_textManager->GetFont(m_chalkFontId);
	char** messages;
	if (m_networkMode != 0) {
		messages = g_apSuccFailNetWin;
		if (m_variant == 0) {
			messages = g_apSuccFailNetLose;
		}
	}
	else {
		messages = g_apSuccFailSingleWin;
		if (m_variant == 0) {
			messages = g_apSuccFailSingleLose;
		}
	}
	format = messages[g_pGameStatus->m_skillState];
	hash = strchr(format, '#');
	if (hash != 0) {
		int prefixLen = hash - format;
		if (prefixLen != 0) {
			strncpy(m_message, format, prefixLen);
		}
		m_message[prefixLen] = 0;
		if (g_pActiveConnection != 0) {
			CNetworkGameMessage* opponentMsg = g_pNetworkManager->GetGameMessage(g_pActiveConnection);
			strcat(m_message, opponentMsg->m_gameName);
		}
		strcat(m_message, hash + 1);
	}
	else {
		strcpy(m_message, format);
	}

	short layoutMinX = (short) m_layout->m_messagePosition.m_x;
	short layoutY = (short) m_layout->m_messagePosition.m_y;
	m_firstLine = m_message;
	m_secondLine = 0;
	bool done = false;
	short lineX;
	short lineHeight;
	CVsSize sizeBuffer;
	CVsSize* textSize;
	do {
		textSize = font->GetSize(&sizeBuffer, m_firstLine, 0x20);
		lineHeight = textSize->m_height;
		lineX = (short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - (int) textSize->m_width) / 2);
		char* prevBreak = (m_secondLine == 0) ? 0 : (m_secondLine - 1);
		if (lineX < layoutMinX) {
			char* space = strrchr(m_firstLine, ' ');
			m_secondLine = space;
			*space = 0;
			m_secondLine = m_secondLine + 1;
			if (prevBreak != 0) {
				*prevBreak = ' ';
			}
		}
		else {
			done = true;
		}
	} while (!done);

	m_firstLinePos.m_x = lineX;
	m_firstLinePos.m_y = layoutY;
	if (m_secondLine == 0) {
		m_firstLinePos.m_y = layoutY + lineHeight / 2;
	}
	else {
		layoutY = layoutY + lineHeight;
		textSize = font->GetSize(&sizeBuffer, m_secondLine, 0x20);
		m_secondLinePos.m_x =
			(short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - (int) textSize->m_width) / 2);
		m_secondLinePos.m_y = layoutY;
	}

	textSize = font->GetSize(&sizeBuffer, g_szPasswordLabel, 0x20);
	short labelHeight = textSize->m_height;
	int labelWidth = textSize->m_width;
	short passwordLabelY = (short) m_layout->m_passwordLabelPosition.m_y;
	int layoutWidth = m_layout->m_frameEnd.m_x;
	int layoutBaseX = m_layout->m_frameStart.m_x;
	m_passwordLabelPos.m_y = passwordLabelY;
	m_passwordLabelPos.m_x = (short) layoutBaseX + (short) ((layoutWidth - (int) labelWidth) / 2);

	textSize = font->GetSize(&sizeBuffer, m_password, 0x20);
	labelWidth = textSize->m_width;
	layoutWidth = m_layout->m_frameEnd.m_x;
	layoutBaseX = m_layout->m_frameStart.m_x;
	m_passwordPos.m_y = passwordLabelY + labelHeight;
	m_passwordPos.m_x = (short) layoutBaseX + (short) ((layoutWidth - (int) labelWidth) / 2);
}

// FUNCTION: LEMBALL 0x00450460
void CSuccFailDrawer::Load()
{
	unsigned long* returnAnim;
	unsigned long* goAnim;
	SuccFailDrawerFieldViews::CPrimitive* primitive;
	SuccFailDrawerFieldViews::DrawerPrims* primary;
	CResBitmap* resource;
	unsigned int position;
	int bitmapX;
	int i;

	if (m_mode != 0) {
		m_layout = (SuccFailLayout*) g_abSuccFailLayoutCompact;
		returnAnim = (unsigned long*) &g_dwSuccFailReturnAnimIdsCompact;
		goAnim = (unsigned long*) &g_dwSuccFailGoAnimIdsCompact;
		if (m_variant != 0) {
			m_backgroundId = RES_NEWFRONT_ANIMS_LORES_SUCCESS_EYES;
			m_primaryBitmapId = g_dwSuccFailSingleLoseBitmapIdCompact;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_LORES_SUCCESS_BOARD;
		}
		else {
			m_primaryBitmapId = g_dwSuccFailSingleWinBitmapIdCompact;
			m_backgroundId = RES_NEWFRONT_ANIMS_LORES_FAIL_EYES;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_LORES_FAILURE_BOARD;
		}
	}
	else {
		m_layout = (SuccFailLayout*) g_abSuccFailLayoutFull;
		returnAnim = (unsigned long*) &g_dwSuccFailReturnAnimIdsFull;
		goAnim = (unsigned long*) &g_dwSuccFailGoAnimIdsFull;
		if (m_variant != 0) {
			m_backgroundId = RES_NEWFRONT_ANIMS_HIRES_SUCCESS_EYES;
			m_primaryBitmapId = g_dwSuccFailSingleLoseBitmapIdFull;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_HIRES_SUCCESS_BOARD;
		}
		else {
			m_backgroundId = RES_NEWFRONT_ANIMS_HIRES_FAIL_EYES;
			m_primaryBitmapId = g_dwSuccFailSingleWinBitmapIdFull;
			m_secondaryBitmapId = RES_NEWFRONT_BITMAPS_HIRES_FAILURE_BOARD;
		}
	}
	m_primaryBitmap = CResBitmap::Load(m_primaryBitmapId);
	if (m_animationsEnabled == 0) {
		m_secondaryBitmap = CResBitmap::Load(m_secondaryBitmapId);
	}
	else {
		m_secondaryBitmap = 0;
	}
	bitmapX = (int) m_width - (int) (short) m_primaryBitmap->m_x;
	primitive = reinterpret_cast<SuccFailDrawerFieldViews::CPrimitive*>(&m_primitiveBundle->m_primitive.m_x);
	primary = reinterpret_cast<SuccFailDrawerFieldViews::DrawerPrims*>(&m_primitives->m_primary.m_x);
	i = 1;
	do {
		resource = m_backgroundBitmap;
		primitive->m_primitive.m_x = m_width - resource->m_x;
		primitive->m_primitive.m_y = (short) m_layout->m_backgroundPosition.m_y;
		primitive->m_primitive.m_resource = resource;
		primitive->m_primitive.m_flags = 0x800;
		primitive->m_primitive.m_remap = 0;
		position = m_layout->m_primaryPosition.m_y;
		primary->m_primary.m_x = (short) bitmapX;
		primary->m_primary.m_y = position;
		resource = m_primaryBitmap;
		primary->m_primary.m_resource = resource;
		primary->m_primary.m_flags = 0x800;
		primary->m_primary.m_remap = 0;
		if (m_secondaryBitmap != 0) {
			position = m_layout->m_secondaryPosition.m_y;
			primary->m_secondary.m_x = (short) m_layout->m_secondaryPosition.m_x;
			primary->m_secondary.m_y = position;
			primary->m_secondary.m_resource = m_secondaryBitmap;
			primary->m_secondary.m_flags = 0x800;
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
	if (m_variant != 0) {
		m_animPosition.m_x = (short) m_layout->m_primaryPosition.m_x + (short) m_layout->m_successAnimOffset.m_x;
		m_animPosition.m_y = (short) m_layout->m_primaryPosition.m_y + (short) m_layout->m_successAnimOffset.m_y;
	}
	else {
		m_animPosition.m_x = (short) m_layout->m_failurePosition.m_x + (short) m_layout->m_failureAnimOffset.m_x;
		m_animPosition.m_y = (short) m_layout->m_failurePosition.m_y + (short) m_layout->m_failureAnimOffset.m_y;
	}
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
	CVsSize advance;

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
	switch ((unsigned int) p_message->type) {
	case 0xc:
		break;
	default:
		m_processedCount++;
		return 0;
	}

	switch ((unsigned int) p_message->code) {
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
				CVsRect rect((short) layout->m_secondaryPosition.m_x,
							 (short) layout->m_secondaryPosition.m_y,
							 (short) layout->m_animWindowEnd.m_x,
							 (short) layout->m_animWindowEnd.m_y);
				m_animWindow.Create(rect, (CPvGWnd*) m_display, g_szPaintballSequence);
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
		if (m_variant != 0) {
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
