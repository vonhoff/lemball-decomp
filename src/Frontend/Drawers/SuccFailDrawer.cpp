#include "SuccFailDrawer.h"

#include "../../Control/Game/GameMain.h"
#include "../../Control/Game/GameStatus.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Animation/StatManager.h"
#include "../../Visos/Foundation/Arena.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../Base/BaseFrontendProcess.h"
#include "../Controls/HiliteController.h"

#include "../../Network/Game/NetworkManager.h"
#include "../../Network/Messages/NetworkGameMessage.h"
#include "../../Visos/Network/Connect.h"
#include "../../Visos/Resources/ResFont.h"
#include "../../Visos/Graphics/BitmapRes.h"
#include <string.h>

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

// GLOBAL: LEMBALL 0x0049fb38
char* g_apSuccFailSingleWin[8] = {
	0, 0, g_szSuccFailCollectedAllFlags, 0, 0, 0, 0, 0
};

// GLOBAL: LEMBALL 0x0049fb58
char* g_apSuccFailNetWin[8] = {
	0,
	g_szSuccFailBeatScore,
	g_szSuccFailCollectedAllYourFlags,
	g_szSuccFailSplattedAllLemmings,
	g_szSuccFailRanOutOfTimeNet,
	g_szSuccFailGaveUpNet,
	0,
	0
};

// GLOBAL: LEMBALL 0x0049fb78
char* g_apSuccFailSingleLose[8] = {
	0, 0, 0, g_szSuccFailAllLemmingsEliminated, g_szSuccFailRanOutOfTimeSingle, g_szSuccFailGaveUpSingle, 0, 0
};

// GLOBAL: LEMBALL 0x0049fb98
char* g_apSuccFailNetLose[8] = {
	0,
	g_szSuccFailOpponentBeatScore,
	g_szSuccFailOpponentCollectedFlags,
	g_szSuccFailOpponentSplattedLemmings,
	g_szSuccFailRanOutOfTimeLose,
	g_szSuccFailGaveUpLose,
	0,
	0
};

// GLOBAL: LEMBALL 0x0049fbb8
unsigned char g_abSuccFailLayoutFull[0x68] = {
	0x5c, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00,
	0x91, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00,
	0x93, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
	0x10, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x32, 0x01, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049fc20
unsigned char g_abSuccFailLayoutCompact[0x68] = {
	0x2e, 0x00, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x00, 0x00,
	0x49, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00,
	0x49, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x90, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049fc88
char g_szPasswordLabel[] = "Password: ";

// GLOBAL: LEMBALL 0x0049fc94
unsigned long g_dwSuccFailReturnAnimIdsFull = 0x1b3;

// GLOBAL: LEMBALL 0x0049fc98
unsigned long g_dwSuccFailGoAnimIdsFull = 0x1b6;

// GLOBAL: LEMBALL 0x0049fc9c
unsigned long g_dwSuccFailReturnAnimIdsCompact = 0x1e7;

// GLOBAL: LEMBALL 0x0049fca0
unsigned long g_dwSuccFailGoAnimIdsCompact = 0x1ea;

// GLOBAL: LEMBALL 0x0049fca4
unsigned long g_dwSuccFailSingleWinBitmapIdFull = 0x135;

// GLOBAL: LEMBALL 0x0049fca8
unsigned long g_dwSuccFailSingleWinBitmapIdCompact = 0x140;

// GLOBAL: LEMBALL 0x0049fcac
unsigned long g_dwSuccFailSingleLoseBitmapIdFull = 0x134;

// GLOBAL: LEMBALL 0x0049fcb0
unsigned long g_dwSuccFailSingleLoseBitmapIdCompact = 0x13f;

extern char g_szMoviePrefix[];

// 68K 0x10809058 __ct__15CSuccFailDrawerFP14CMain2DDisplayP4CGDIRC7CVSRectUc
// FUNCTION: LEMBALL 0x00450020
SuccFailDrawer::SuccFailDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2, unsigned char p_arg3)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 0xe, 0x32, 200, 0, 0x28, 0x30)
{
	m_firstLinePos.m_y = 0;
	m_firstLinePos.m_x = 0;
	m_secondLinePos.m_y = 0;
	m_secondLinePos.m_x = 0;
	m_passwordLabelPos.m_y = 0;
	m_passwordLabelPos.m_x = 0;
	m_passwordPos.m_y = 0;
	m_passwordPos.m_x = 0;
	m_variant = p_arg3;
	m_animationsEnabled = (unsigned int) (g_nAnimationsDisabled == 0);
	m_animWindow.Initialise(this, m_display, p_arg3);
	m_animWindow.m_resolveMoviePath = 1;
	m_animWindow.m_moviePrefix = g_szMoviePrefix;
	m_animWindow.m_useMoviePrefix = 1;
	m_animStarted = 0;
	m_soundStarted = 0;
	m_soundStopped = 0;
	m_drawBackground = 1;
	m_animStartDeadline = CurrentMilliTimer() + 0x28;
	m_drawFrame = 1;
	m_drawSolid = 1;
	m_password = g_pGameStatus->EncodePassword();
	Setup();
}

// 68K 0x1080919a CalculateText__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450160
void SuccFailDrawer::CalculateText()
{
	ResFont* font;
	char* format;
	char* hash;

	font = m_textManager->GetFont(m_unknown384);
	if (m_networkMode == 0) {
		if (m_variant == 0) {
			format = g_apSuccFailSingleLose[g_pGameStatus->m_skillState];
		}
		else {
			format = g_apSuccFailSingleWin[g_pGameStatus->m_skillState];
		}
	}
	else {
		if (m_variant == 0) {
			format = g_apSuccFailNetLose[g_pGameStatus->m_skillState];
		}
		else {
			format = g_apSuccFailNetWin[g_pGameStatus->m_skillState];
		}
	}

	hash = strchr(format, '#');
	if (hash == 0) {
		strcpy(m_message, format);
	}
	else {
		int prefixLen = hash - format;
		if (prefixLen != 0) {
			memcpy(m_message, format, prefixLen);
		}
		m_message[prefixLen] = 0;
		if (g_pActiveConnection != 0) {
			NetworkGameMessage* opponentMsg = g_pNetworkManager->GetGameMessage(g_pActiveConnection);
			strcat(m_message, opponentMsg->m_gameName);
		}
		strcat(m_message, hash + 1);
	}

	int* layout = (int*) m_layout;
	short layoutMinX = (short) layout[0x48 / 4];
	short layoutY = (short) layout[0x4c / 4];
	m_firstLine = m_message;
	m_secondLine = 0;
	bool done = false;
	short lineX;
	VsPoint textSize;
	do {
		textSize = font->GetSize(m_firstLine, 0x20);
		lineX = (short) layout[0x38 / 4] + (short) ((layout[0x40 / 4] - (int) textSize.m_x) / 2);
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
		m_firstLinePos.m_y = layoutY + textSize.m_y / 2;
	}
	else {
		layoutY = layoutY + textSize.m_y;
		textSize = font->GetSize(m_secondLine, 0x20);
		m_secondLinePos.m_x = (short) layout[0x38 / 4] + (short) ((layout[0x40 / 4] - (int) textSize.m_x) / 2);
		m_secondLinePos.m_y = layoutY;
	}

	textSize = font->GetSize(g_szPasswordLabel, 0x20);
	short labelHeight = textSize.m_y;
	short labelWidth = textSize.m_x;
	short passwordLabelY = (short) layout[0x64 / 4];
	int layoutWidth = layout[0x40 / 4];
	int layoutBaseX = layout[0x38 / 4];
	m_passwordLabelPos.m_y = passwordLabelY;
	m_passwordLabelPos.m_x = (short) layoutBaseX + (short) ((layoutWidth - (int) labelWidth) / 2);

	textSize = font->GetSize(m_password, 0x20);
	labelWidth = textSize.m_x;
	m_passwordPos.m_y = passwordLabelY + labelHeight;
	m_passwordPos.m_x = (short) layoutBaseX + (short) ((layoutWidth - (int) labelWidth) / 2);
}

// 68K 0x10809508 Load__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450460
void SuccFailDrawer::Load()
{
	unsigned long* goAnim;
	unsigned long* returnAnim;
	int* layout;

	if (m_mode == 0) {
		m_layout = g_abSuccFailLayoutFull;
		goAnim = (unsigned long*) &g_dwSuccFailGoAnimIdsFull;
		returnAnim = (unsigned long*) &g_dwSuccFailReturnAnimIdsFull;
		if (m_variant == 0) {
			m_backgroundId = 0x157;
			m_primaryBitmapId = g_dwSuccFailSingleWinBitmapIdFull;
			m_secondaryBitmapId = 0x137;
		}
		else {
			m_backgroundId = 0x158;
			m_primaryBitmapId = g_dwSuccFailSingleLoseBitmapIdFull;
			m_secondaryBitmapId = 0x138;
		}
	}
	else {
		m_layout = g_abSuccFailLayoutCompact;
		goAnim = (unsigned long*) &g_dwSuccFailGoAnimIdsCompact;
		returnAnim = (unsigned long*) &g_dwSuccFailReturnAnimIdsCompact;
		if (m_variant == 0) {
			m_primaryBitmapId = g_dwSuccFailSingleWinBitmapIdCompact;
			m_backgroundId = 0x184;
			m_secondaryBitmapId = 0x142;
		}
		else {
			m_backgroundId = 0x185;
			m_primaryBitmapId = g_dwSuccFailSingleLoseBitmapIdCompact;
			m_secondaryBitmapId = 0x143;
		}
	}
	m_primaryBitmap = ResBitmap::Load(m_primaryBitmapId);
	if (m_animationsEnabled == 0) {
		m_secondaryBitmap = ResBitmap::Load(m_secondaryBitmapId);
	}
	else {
		m_secondaryBitmap = 0;
	}
	layout = (int*) m_layout;
	int bitmapX = (int) m_width - (int) (short) m_primaryBitmap->m_x;
	m_primitiveBundle.m_primitive.m_x = m_width - m_backgroundBitmap->m_x;
	m_primitiveBundle.m_primitive.m_y = (short) layout[0x14 / 4];
	m_primitiveBundle.m_primitive.m_resource = m_backgroundBitmap;
	m_primitiveBundle.m_primitive.m_flags = 0x800;
	m_primitives[0].m_primary.m_x = (short) bitmapX;
	m_primitives[0].m_primary.m_y = (short) layout[0x1c / 4];
	m_primitives[0].m_primary.m_resource = m_primaryBitmap;
	m_primitives[0].m_primary.m_flags = 8;
	if (m_secondaryBitmap != 0) {
		m_primitives[0].m_secondary.m_x = (short) layout[0x50 / 4];
		m_primitives[0].m_secondary.m_y = (short) layout[0x54 / 4];
		m_primitives[0].m_secondary.m_resource = m_secondaryBitmap;
		m_primitives[0].m_secondary.m_flags = 8;
	}
	layout[0x18 / 4] = bitmapX;
	layout[0x28 / 4] = bitmapX;
	m_buttonBinding = 0;
	m_hiliteController = new HiliteController((GWnd*) m_display, m_gdi, 2, (unsigned char) m_mode, 0);
	m_hiliteController->AddButton(layout[0], layout[1], returnAnim, 1, 0, 0, 0, &m_buttonBinding, 0xacef0010);
	m_hiliteController->AddButton(layout[2], layout[3], goAnim, 1, 0, 0, 0, &m_buttonBinding, 0xacef0011);
	m_hiliteController->SetHilite(0);
	m_hiliteController->SetHiliteWindow();
	if (m_variant == 0) {
		m_animPosition.m_x = (short) layout[0x28 / 4] + (short) layout[0x30 / 4];
		m_animPosition.m_y = (short) layout[0x2c / 4] + (short) layout[0x34 / 4];
	}
	else {
		m_animPosition.m_x = (short) layout[0x18 / 4] + (short) layout[0x20 / 4];
		m_animPosition.m_y = (short) layout[0x1c / 4] + (short) layout[0x24 / 4];
	}
	CalculateText();
	if (m_animationsEnabled != 0) {
		m_animWindow.SetVariant(m_mode);
	}
}

// GLOBAL: LEMBALL 0x0049fe18
char g_szPaintballSequence[] = "Paintball Sequence";

// 68K 0x10809888 UnLoad__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450770
void SuccFailDrawer::UnLoad()
{
	if (m_hiliteController != 0) {
		delete m_hiliteController;
	}
	m_primaryBitmap->UnLoad();
	if (m_secondaryBitmap != 0) {
		m_secondaryBitmap->UnLoad();
	}
}

// 68K 0x1080999a DestroyDrawer__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450820
void SuccFailDrawer::DestroyDrawer()
{
	if (m_animStarted != 0 && m_animWindow.m_lifecycleRefs == 1) {
		m_animWindow.Destroy();
		m_animStarted = 0;
		m_animStartDeadline = CurrentMilliTimer() + 0x28;
	}
}

// 68K 0x108099c8 DrawText__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450860
void SuccFailDrawer::DrawText()
{
	VsSize advance;

	if (m_drawingBackBuffer != 0) {
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi, m_firstLinePos, advance, m_unknown384, m_firstLine, 0x20, 0);
		if (m_secondLine != 0) {
			advance.m_height = 0;
			advance.m_width = 0;
			m_textManager->DrawString(m_gdi, m_secondLinePos, advance, m_unknown384, m_secondLine, 0x20, 0);
		}
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi, m_passwordLabelPos, advance, m_unknown384, g_szPasswordLabel, 0x20, 0);
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi, m_passwordPos, advance, m_unknown384, m_password, 0x20, 0);
	}
}

// 68K 0x10809ac4 ProcessMessages__15CSuccFailDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00450970
bool SuccFailDrawer::ProcessMessages(Message* p_message)
{
	if ((unsigned int) p_message->type != 0xc) {
		m_processedCount++;
		return 0;
	}

	switch ((unsigned int) p_message->code) {
	case 0xacef0010:
		if (m_networkMode != 0) {
			Action(3, 0);
			return 1;
		}
		Go();
		return 1;

	case 0xacef0011:
		if (m_networkMode != 0) {
			Action(2, 0);
			return 1;
		}
		Return();
		return 1;

	default:
		return 0;
	}
}

// 68K 0x10809b8c Return__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x004509f0
void SuccFailDrawer::Return()
{
	m_quitYet = 1;
	m_returnState = 2;
}

// 68K 0x10809bc2 Go__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450a10
void SuccFailDrawer::Go()
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

// 68K 0x10809c48 ConfirmedAction__15CSuccFailDrawerF12eUserActions
// FUNCTION: LEMBALL 0x00450a60
bool SuccFailDrawer::ConfirmedAction(int p_action)
{
	switch (p_action) {
	case 2:
		Go();
		return 1;
	case 3:
		Return();
		return 1;
	default:
		return 0;
	}
}

// 68K 0x10809cb4 Processing__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450a90
void SuccFailDrawer::Processing()
{
	unsigned long now;
	int* layout;

	if (m_animStarted == 0) {
		now = CurrentMilliTimer();
		if (now > m_animStartDeadline && m_animationsEnabled != 0) {
			if (m_display->IsWindowValid() != 0) {
				layout = (int*) m_layout;
				VsRect rect((short) layout[0x50 / 4], (short) layout[0x54 / 4], (short) layout[0x58 / 4], (short) layout[0x5c / 4]);
				m_animWindow.Create(rect, (PvGWnd*) m_display, g_szPaintballSequence);
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
		if (m_variant == 0) {
			g_pSoundView->PlayEffect((eSoundEffect) 0x28);
		}
		else {
			g_pSoundView->PlayEffect((eSoundEffect) 0x27);
		}
		m_soundStarted = 1;
		m_soundStartTime = CurrentMilliTimer();
	}
}

// 68K 0x10809d96 DrawBackGround__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x00450ba0
void SuccFailDrawer::DrawBackGround()
{
	int* layout = (int*) m_layout;
	CoordPair* start = (CoordPair*) &layout[0x38 / 4];
	CoordPair* end = (CoordPair*) &layout[0x40 / 4];
	DrawFrame(*start, *end);
	m_primitives[m_primitiveBank].m_primary.Draw(m_gdi);
	if (m_secondaryBitmap != 0) {
		m_primitives[m_primitiveBank].m_secondary.Draw(m_gdi);
	}
}

// 68K 0x108098f2 __dt__15CSuccFailDrawerFv
SuccFailDrawer::~SuccFailDrawer()
{
}

