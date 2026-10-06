#include "CSuccFailDrawer.h"

#include "Application/CGameStatus.h"
#include "Application/GameMain.h"
#include "Engine/Memory/CArena.h"
#include "Engine/Resources/Manifest.h"
#include "Engine/Resources/Types/CResBITMAP.h"
#include "Engine/Statistics/CStatManager.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Text/CTextManager.h"
#include "Engine/Time/VsTime.h"
#include "Frontend/CBaseFrontendProcess.h"
#include "Frontend/Controls/CHiliteController.h"
#include "Frontend/FrontendLayoutMode.h"
#include "Frontend/tagPRIMS.h"
#include "GameView/Display/CMain2DDisplay.h"
#include "GameView/Sound/CSoundView.h"
#include "Level/CLevelLoader.h"
#include "Multiplayer/Transport/NetworkMode.h"

extern "C" unsigned long __stdcall timeGetTime(void);

#include "Application/FlowProcesses.h"
#include "Application/SoundEffects.h"
#include "CSuccFailAnimWnd.h"
#include "Engine/Graphics/Primitives/CBigBitmap.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/Message.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/CoordPair.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

class CGWnd;

#define SUCC_FAIL_BUTTON_MESSAGE_GO 0xacef0010
#define SUCC_FAIL_BUTTON_MESSAGE_RETURN 0xacef0011

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
char* g_apSuccFailSingleWin[8] = {NULL, NULL, g_szSuccFailCollectedAllFlags, NULL, NULL, NULL, NULL, NULL};

// GLOBAL: LEMBALL 0x0049fb58
char* g_apSuccFailNetWin[8] = {NULL,
							   g_szSuccFailBeatScore,
							   g_szSuccFailCollectedAllYourFlags,
							   g_szSuccFailSplattedAllLemmings,
							   g_szSuccFailRanOutOfTimeNet,
							   g_szSuccFailGaveUpNet,
							   NULL,
							   NULL};

// GLOBAL: LEMBALL 0x0049fb78
char* g_apSuccFailSingleLose[8] = {NULL,
								   NULL,
								   NULL,
								   g_szSuccFailAllLemmingsEliminated,
								   g_szSuccFailRanOutOfTimeSingle,
								   g_szSuccFailGaveUpSingle,
								   NULL,
								   NULL};

// GLOBAL: LEMBALL 0x0049fb98
char* g_apSuccFailNetLose[8] = {NULL,
								g_szSuccFailOpponentBeatScore,
								g_szSuccFailOpponentCollectedFlags,
								g_szSuccFailOpponentSplattedLemmings,
								g_szSuccFailRanOutOfTimeLose,
								g_szSuccFailGaveUpLose,
								NULL,
								NULL};

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

// GLOBAL: LEMBALL 0x0049fe18
char g_szPaintballSequence[] = "Paintball Sequence";

#include "Engine/Time/VsTime.h"

extern char g_szSuccFailMoviePrefix[];

extern "C" unsigned long __stdcall timeGetTime(void);

#include "Engine/Resources/Types/CResFONT.h"
#include "Multiplayer/CNetworkGameMessage.h"
#include "Multiplayer/CNetworkManager.h"
#include "Multiplayer/Transport/CConnect.h"

#include <string.h>

class CGWnd;

#pragma intrinsic(strcpy, strlen)

extern char* g_apSuccFailSingleWin[8];
extern char* g_apSuccFailNetWin[8];
extern char* g_apSuccFailSingleLose[8];
extern char* g_apSuccFailNetLose[8];
extern char g_szPasswordLabel[];

// FUNCTION: LEMBALL 0x00450020
CSuccFailDrawer::CSuccFailDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect, unsigned int p_success)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_SUCCESS, 0x32, 200, 0, 0x28, 0x30)
{
	m_success = p_success;
	m_animationsEnabled = (unsigned int) (g_nAnimationsDisabled == 0);
	m_animWindow.Initialise(this, m_display, p_success);
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
	CResFONT* font;
	char* format;
	char* hash;

	font = m_textManager->GetFont(m_chalkFontId);
	char** messages;
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		messages = g_apSuccFailNetWin;
		if (m_success == 0) {
			messages = g_apSuccFailNetLose;
		}
	}
	else {
		messages = g_apSuccFailSingleWin;
		if (m_success == 0) {
			messages = g_apSuccFailSingleLose;
		}
	}
	format = messages[g_pGameStatus->m_skillState];
	hash = strchr(format, '#');
	if (hash != NULL) {
		int prefixLen = hash - format;
		if (prefixLen != 0) {
			strncpy(m_message, format, prefixLen);
		}
		m_message[prefixLen] = 0;
		if (g_pActiveConnection != NULL) {
			CNetworkGameMessage* opponentMsg = g_pNetworkManager->GetGameMessage(g_pActiveConnection);
			strcat(m_message, opponentMsg->m_gameName);
		}
		strcat(m_message, hash + 1);
	}
	else {
		strcpy(m_message, format);
	}

	{
		bool done = false;
		short layoutMinX = (short) m_layout->m_messagePosition.m_x;
		short layoutY = (short) m_layout->m_messagePosition.m_y;
		m_firstLine = m_message;
		m_secondLine = NULL;
		short lineX;
		CVSSize measuredSize;
		do {
			const CVSSize& textSize = font->GetSize(m_firstLine, TEXT_ADVANCE_X_POSITIVE);
			measuredSize.m_height = textSize.m_height;
			measuredSize.m_width = textSize.m_width;
			lineX = (short) m_layout->m_frameStart.m_x +
					(short) ((m_layout->m_frameEnd.m_x - (int) measuredSize.m_width) / 2);
			char* prevBreak = (m_secondLine != NULL) ? (m_secondLine - 1) : NULL;
			if (lineX < layoutMinX) {
				char* space = strrchr(m_firstLine, ' ');
				m_secondLine = space;
				*space = 0;
				m_secondLine = m_secondLine + 1;
				if (prevBreak != NULL) {
					*prevBreak = ' ';
				}
			}
			else {
				done = true;
			}
		} while (!done);

		m_firstLinePos.m_x = lineX;
		m_firstLinePos.m_y = layoutY;
		if (m_secondLine == NULL) {
			m_firstLinePos.m_y = layoutY + measuredSize.m_height / 2;
		}
		else {
			layoutY = layoutY + measuredSize.m_height;
			const CVSSize& textSize = font->GetSize(m_secondLine, TEXT_ADVANCE_X_POSITIVE);
			m_secondLinePos.m_x =
				(short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - (int) textSize.m_width) / 2);
			m_secondLinePos.m_y = layoutY;
		}
	}
	short passwordLabelY;
	{
		const CVSSize& textSize = font->GetSize(g_szPasswordLabel, TEXT_ADVANCE_X_POSITIVE);
		short labelHeight = textSize.m_height;
		int labelWidth = textSize.m_width;
		passwordLabelY = (short) m_layout->m_passwordLabelPosition.m_y;
		m_passwordLabelPos.m_x =
			(short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - labelWidth) / 2);
		m_passwordLabelPos.m_y = passwordLabelY;
		passwordLabelY += labelHeight;
	}

	{
		const CVSSize& passwordSize = font->GetSize(m_password, TEXT_ADVANCE_X_POSITIVE);
		int labelWidth = passwordSize.m_width;
		m_passwordPos.m_x = (short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - labelWidth) / 2);
		m_passwordPos.m_y = passwordLabelY;
	}
}

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

	if (m_mode != FRONTEND_LAYOUT_STANDARD) {
		m_layout = &g_succFailLayoutCompact;
		returnAnim = &g_dwSuccFailReturnAnimIdsCompact;
		goAnim = &g_dwSuccFailGoAnimIdsCompact;
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
		returnAnim = &g_dwSuccFailReturnAnimIdsFull;
		goAnim = &g_dwSuccFailGoAnimIdsFull;
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
		m_secondaryBitmap = NULL;
	}
	bitmapX = m_size.m_width - m_primaryBitmap->m_x;
	primitive = m_primitiveBundle;
	primary = m_primitives;
	i = 1;
	do {
		resource = m_backgroundBitmap;
		position = m_layout->m_backgroundPosition.m_y;
		primitive->m_primitive.m_x = m_size.m_width - resource->m_x;
		primitive->m_primitive.m_y = position;
		primitive->m_primitive.m_resource = resource;
		primitive->m_primitive.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		primitive->m_primitive.m_remap = NULL;
		resource = m_primaryBitmap;
		position = m_layout->m_primaryPosition.m_y;
		primary->m_primary.m_x = (short) bitmapX;
		primary->m_primary.m_y = position;
		primary->m_primary.m_resource = resource;
		primary->m_primary.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		primary->m_primary.m_remap = NULL;
		CResBITMAP* secondaryResource = m_secondaryBitmap;
		if (secondaryResource != NULL) {
			unsigned int secondaryY;
			SuccFailLayout* layout = m_layout;
			secondaryY = layout->m_secondaryPosition.m_y;
			unsigned int secondaryX = layout->m_secondaryPosition.m_x;
			primary->m_secondary.m_x = (short) secondaryX;
			primary->m_secondary.m_y = secondaryY;
			primary->m_secondary.m_resource = secondaryResource;
			primary->m_secondary.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
			primary->m_secondary.m_remap = NULL;
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
								  SUCC_FAIL_BUTTON_MESSAGE_GO);
	m_hiliteController->AddButton(m_layout->m_goButton.m_x,
								  m_layout->m_goButton.m_y,
								  returnAnim,
								  1,
								  0,
								  0,
								  0,
								  &m_buttonBinding,
								  SUCC_FAIL_BUTTON_MESSAGE_RETURN);
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

// FUNCTION: LEMBALL 0x00450770
void CSuccFailDrawer::UnLoad()
{
	if (m_hiliteController != NULL) {
		delete m_hiliteController;
	}
	m_primaryBitmap->UnLoad();
	if (m_secondaryBitmap != NULL) {
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
		m_textManager
			->DrawString(m_gdi, m_firstLinePos, advance, m_chalkFontId, m_firstLine, TEXT_ADVANCE_X_POSITIVE, NULL);
		if (m_secondLine != NULL) {
			advance.m_height = 0;
			advance.m_width = 0;
			m_textManager->DrawString(m_gdi,
									  m_secondLinePos,
									  advance,
									  m_chalkFontId,
									  m_secondLine,
									  TEXT_ADVANCE_X_POSITIVE,
									  NULL);
		}
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi,
								  m_passwordLabelPos,
								  advance,
								  m_chalkFontId,
								  g_szPasswordLabel,
								  TEXT_ADVANCE_X_POSITIVE,
								  NULL);
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager
			->DrawString(m_gdi, m_passwordPos, advance, m_chalkFontId, m_password, TEXT_ADVANCE_X_POSITIVE, NULL);
	}
}

// FUNCTION: LEMBALL 0x00450970
bool CSuccFailDrawer::ProcessMessages(Message* p_message)
{
	switch ((unsigned int) p_message->m_type) {
	case MESSAGE_BUTTON_RELEASED:
		break;
	default:
		m_processedCount++;
		return false;
	}

	switch ((unsigned int) p_message->m_code) {
	case SUCC_FAIL_BUTTON_MESSAGE_GO:
		if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
			Action(USER_ACTION_SUCC_FAIL_GO_REQUEST, USER_ACTION_STAGE_REQUEST);
			return true;
		}
		Go();
		return true;

	case SUCC_FAIL_BUTTON_MESSAGE_RETURN:
		if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
			Action(USER_ACTION_SUCC_FAIL_RETURN_REQUEST, USER_ACTION_STAGE_REQUEST);
			return true;
		}
		Return();
		return true;

	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x004509f0
void CSuccFailDrawer::Return()
{
	m_quitYet = 1;
	m_returnState = FLOW_MAIN_OPTIONS_1;
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
	m_returnState = FLOW_PREVIEW;
}

// FUNCTION: LEMBALL 0x00450a60
bool CSuccFailDrawer::ConfirmedAction(eUserActions p_action)
{
	switch (p_action) {
	case USER_ACTION_SUCC_FAIL_RETURN_CONFIRM:
		Return();
		return true;
	case USER_ACTION_SUCC_FAIL_GO_CONFIRM:
		Go();
		return true;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x00450a90
void CSuccFailDrawer::Processing()
{
	unsigned long now;
	SuccFailLayout* layout;

	if (m_animStarted == 0) {
		now = CurrentMilliTimer();
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
		if (skill != SKILL_MAYHEM || g_pGameStatus->m_lastLevels[skill] != SKILL_LEVEL_COUNT_MAYHEM) {
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
		m_soundStartTime = CurrentMilliTimer();
	}
}

// FUNCTION: LEMBALL 0x00450ba0
void CSuccFailDrawer::DrawBackGround()
{
	SuccFailLayout* layout = m_layout;
	DrawFrame(layout->m_frameStart, layout->m_frameEnd);
	m_primitives[m_primitiveBank].m_primary.Draw(m_gdi);
	if (m_secondaryBitmap != NULL) {
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
